#include "cgpa/DatabaseManager.hpp"
#include "cgpa/Course.hpp"
#include "cgpa/Semester.hpp"
#include "cgpa/Student.hpp"
#include "sqlite/sqlite3.h"
#include <stdexcept>
#include <map>
#include <vector>

namespace cgpa {

// ----------------------------------------------------------------
// Constructeur / Destructeur
// ----------------------------------------------------------------

DatabaseManager::DatabaseManager(const std::string& dbPath) : db_(nullptr), dbPath_(dbPath) {
    int rc = sqlite3_open(dbPath.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw std::runtime_error("Impossible d'ouvrir la base SQLite : " + err);
    }
    createSchema();
}

DatabaseManager::~DatabaseManager() {
    if (db_) sqlite3_close(db_);
}

// ----------------------------------------------------------------
// Helpers internes
// ----------------------------------------------------------------

void DatabaseManager::execSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string err = errMsg ? errMsg : "erreur inconnue";
        sqlite3_free(errMsg);
        throw std::runtime_error("Erreur SQL : " + err + "\nSQL : " + sql);
    }
}

// ----------------------------------------------------------------
// Création du schéma (idempotent grâce à IF NOT EXISTS)
// ----------------------------------------------------------------

void DatabaseManager::createSchema() {
    execSQL(R"(
        CREATE TABLE IF NOT EXISTS students (
            id   TEXT PRIMARY KEY,
            name TEXT NOT NULL
        );
        CREATE TABLE IF NOT EXISTS semesters (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            student_id TEXT    NOT NULL REFERENCES students(id) ON DELETE CASCADE,
            label      TEXT    NOT NULL,
            position   INTEGER NOT NULL DEFAULT 0
        );
        CREATE TABLE IF NOT EXISTS courses (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            semester_id INTEGER NOT NULL REFERENCES semesters(id) ON DELETE CASCADE,
            name        TEXT    NOT NULL,
            credits     REAL    NOT NULL,
            letter_grade TEXT   NOT NULL,
            position    INTEGER NOT NULL DEFAULT 0
        );
        PRAGMA foreign_keys = ON;
    )");
}

// ----------------------------------------------------------------
// insertStudent : ajoute un étudiant + ses semestres + ses cours
// dans une transaction atomique.
// ----------------------------------------------------------------

void DatabaseManager::insertStudent(const Student& student) {
    execSQL("BEGIN TRANSACTION;");
    try {
        // Insérer l'étudiant
        std::string sql = "INSERT INTO students (id, name) VALUES ('"
                        + student.getId() + "', '"
                        + student.getName() + "');";
        execSQL(sql);

        // Insérer les semestres
        int semPos = 0;
        for (const auto& sem : student.getSemesters()) {
            std::string semSql =
                "INSERT INTO semesters (student_id, label, position) VALUES ('"
                + student.getId() + "', '"
                + sem.getLabel() + "', "
                + std::to_string(semPos++) + ");";
            execSQL(semSql);

            // Récupérer l'ID auto-généré du semestre
            long long semId = sqlite3_last_insert_rowid(db_);

            // Insérer les cours
            int coursePos = 0;
            for (const auto& course : sem.getCourses()) {
                std::string cSql =
                    "INSERT INTO courses (semester_id, name, credits, letter_grade, position) VALUES ("
                    + std::to_string(semId) + ", '"
                    + course.getName() + "', "
                    + std::to_string(course.getCreditHours()) + ", '"
                    + course.getLetterGrade() + "', "
                    + std::to_string(coursePos++) + ");";
                execSQL(cSql);
            }
        }

        execSQL("COMMIT;");
    } catch (...) {
        execSQL("ROLLBACK;");
        throw;
    }
}

// ----------------------------------------------------------------
// deleteStudent
// ----------------------------------------------------------------

bool DatabaseManager::deleteStudent(const std::string& id) {
    std::string sql = "DELETE FROM students WHERE id = '" + id + "';";
    execSQL(sql);
    return sqlite3_changes(db_) > 0;
}

// ----------------------------------------------------------------
// save : vide la base et réinsère tout (remplacement complet)
// ----------------------------------------------------------------

void DatabaseManager::save(const StudentManager& manager) {
    execSQL("BEGIN TRANSACTION;");
    try {
        execSQL("DELETE FROM courses;");
        execSQL("DELETE FROM semesters;");
        execSQL("DELETE FROM students;");

        for (const auto& student : manager.getAll()) {
            // On réutilise insertStudent mais sans transaction imbriquée
            int semPos = 0;
            std::string sql = "INSERT INTO students (id, name) VALUES ('"
                            + student.getId() + "', '"
                            + student.getName() + "');";
            execSQL(sql);

            for (const auto& sem : student.getSemesters()) {
                std::string semSql =
                    "INSERT INTO semesters (student_id, label, position) VALUES ('"
                    + student.getId() + "', '"
                    + sem.getLabel() + "', "
                    + std::to_string(semPos++) + ");";
                execSQL(semSql);
                long long semId = sqlite3_last_insert_rowid(db_);

                int coursePos = 0;
                for (const auto& course : sem.getCourses()) {
                    std::string cSql =
                        "INSERT INTO courses (semester_id, name, credits, letter_grade, position) VALUES ("
                        + std::to_string(semId) + ", '"
                        + course.getName() + "', "
                        + std::to_string(course.getCreditHours()) + ", '"
                        + course.getLetterGrade() + "', "
                        + std::to_string(coursePos++) + ");";
                    execSQL(cSql);
                }
            }
        }
        execSQL("COMMIT;");
    } catch (...) {
        execSQL("ROLLBACK;");
        throw;
    }
}

// ----------------------------------------------------------------
// load : lit la base et remplit le StudentManager
// ----------------------------------------------------------------

void DatabaseManager::load(StudentManager& manager) {
    // Étape 1 : charger les étudiants (ordre d'insertion)
    struct SemData {
        std::string label;
        int         position;
        long long   id;
    };
    struct CourseData {
        std::string name, letterGrade;
        double      credits;
        int         position;
        long long   semesterId;
    };

    // Charger les étudiants
    std::vector<std::pair<std::string,std::string>> studentRows; // (id, name)
    {
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db_,
            "SELECT id, name FROM students ORDER BY rowid;",
            -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string id   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            studentRows.push_back({id, name});
        }
        sqlite3_finalize(stmt);
    }

    // Charger tous les semestres
    std::map<long long, SemData> semMap;
    std::map<std::string, std::vector<long long>> studentSems; // student_id → [sem_id]
    {
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db_,
            "SELECT id, student_id, label, position FROM semesters ORDER BY student_id, position;",
            -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long long   id        = sqlite3_column_int64(stmt, 0);
            std::string studentId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string label     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int         pos       = sqlite3_column_int(stmt, 3);
            semMap[id] = { label, pos, id };
            studentSems[studentId].push_back(id);
        }
        sqlite3_finalize(stmt);
    }

    // Charger tous les cours
    std::map<long long, std::vector<CourseData>> semCourses; // sem_id → courses
    {
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db_,
            "SELECT semester_id, name, credits, letter_grade, position FROM courses ORDER BY semester_id, position;",
            -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long long   semId  = sqlite3_column_int64(stmt, 0);
            std::string name   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            double      cred   = sqlite3_column_double(stmt, 2);
            std::string grade  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int         pos    = sqlite3_column_int(stmt, 4);
            semCourses[semId].push_back({ name, grade, cred, pos, semId });
        }
        sqlite3_finalize(stmt);
    }

    // Reconstituer les objets domaine
    for (const auto& [id, name] : studentRows) {
        Student student(id, name);

        for (long long semId : studentSems[id]) {
            Semester sem(semMap[semId].label);
            for (const auto& c : semCourses[semId]) {
                sem.addCourse(Course(c.name, c.credits, c.letterGrade));
            }
            student.addSemester(sem);
        }
        manager.addStudent(student);
    }
}

} // namespace cgpa
