#pragma once
#include <string>
#include "cgpa/StudentManager.hpp"

// Forward-declaration pour ne pas exposer sqlite3.h dans les headers publics
struct sqlite3;

namespace cgpa {

// Remplace FileManager (CSV) par une vraie base SQLite.
// Schema :
//   students  (id TEXT PK, name TEXT)
//   semesters (id INT PK AUTOINCREMENT, student_id FK, label TEXT)
//   courses   (id INT PK AUTOINCREMENT, semester_id FK,
//              name TEXT, credits REAL, letter_grade TEXT)
//
// Un seul fichier .db contient tout — portable, pas de serveur.
class DatabaseManager {
public:
    // Ouvre (ou crée) la base au chemin indiqué.
    // Lève std::runtime_error si l'ouverture échoue.
    explicit DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    // Interdit la copie (sqlite3* n'est pas copiable)
    DatabaseManager(const DatabaseManager&)            = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Charge tous les étudiants de la base dans le manager.
    void load(StudentManager& manager);

    // Sauvegarde tout le contenu du manager dans la base
    // (remplace les données existantes via transaction).
    void save(const StudentManager& manager);

    // Ajoute un seul étudiant (utilisé par l'API POST /students).
    void insertStudent(const Student& student);

    // Supprime un étudiant par ID (utilisé par DELETE /students/:id).
    bool deleteStudent(const std::string& id);

private:
    sqlite3*    db_;
    std::string dbPath_;

    void createSchema();
    void execSQL(const std::string& sql);
};

} // namespace cgpa
