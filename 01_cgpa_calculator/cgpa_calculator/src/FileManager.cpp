#include "cgpa/FileManager.hpp"
#include "cgpa/Course.hpp"
#include "cgpa/Semester.hpp"
#include "cgpa/Student.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>

namespace cgpa {

// ----------------------------------------------------------------
// Helpers privés
// ----------------------------------------------------------------

std::vector<std::string> FileManager::splitCsv(const std::string& line, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ----------------------------------------------------------------
// Save
// ----------------------------------------------------------------

void FileManager::save(const StudentManager& manager, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier en ecriture : " + filepath);
    }

    // En-tête
    file << "student_id,student_name,semester,course_name,credits,letter_grade\n";

    for (const auto& student : manager.getAll()) {
        for (const auto& semester : student.getSemesters()) {
            for (const auto& course : semester.getCourses()) {
                file << student.getId()          << ","
                     << student.getName()        << ","
                     << semester.getLabel()      << ","
                     << course.getName()         << ","
                     << course.getCreditHours()  << ","
                     << course.getLetterGrade()  << "\n";
            }
        }
    }

    if (file.fail()) {
        throw std::runtime_error("Erreur d'ecriture dans le fichier : " + filepath);
    }
}

// ----------------------------------------------------------------
// Load
// ----------------------------------------------------------------

void FileManager::load(StudentManager& manager, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier en lecture : " + filepath);
    }

    // Structure intermediaire pour reconstituer etudiants et semestres.
    // On preserve l'ordre d'insertion avec deux vecteurs parallèles :
    // - studentOrder  : liste ordonnee des IDs rencontres
    // - studentData   : id -> (nom, [(semLabel, [(courseName, credits, grade)])])
    std::vector<std::string> studentOrder;
    std::map<std::string, std::pair<std::string,
        std::vector<std::pair<std::string,
            std::vector<std::tuple<std::string, double, std::string>>>>>> studentData;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;

        // Ignorer l'en-tête et les lignes vides
        if (lineNumber == 1 || line.empty()) continue;

        auto tokens = splitCsv(line);
        if (tokens.size() != 6) {
            throw std::runtime_error("Format invalide ligne " + std::to_string(lineNumber)
                                     + " (attendu 6 colonnes, recu "
                                     + std::to_string(tokens.size()) + ")");
        }

        const std::string& sid     = tokens[0];
        const std::string& sname   = tokens[1];
        const std::string& semLbl  = tokens[2];
        const std::string& cname   = tokens[3];
        double             credits = 0;
        const std::string& grade   = tokens[5];

        try {
            credits = std::stod(tokens[4]);
        } catch (...) {
            throw std::runtime_error("Credits invalides ligne " + std::to_string(lineNumber)
                                     + " : '" + tokens[4] + "'");
        }

        // Premier enregistrement de cet etudiant
        if (studentData.find(sid) == studentData.end()) {
            studentOrder.push_back(sid);
            studentData[sid] = { sname, {} };
        }

        // Trouver ou créer le semestre de cet etudiant
        auto& semesters = studentData[sid].second;
        auto semIt = std::find_if(semesters.begin(), semesters.end(),
                                  [&semLbl](const auto& p) { return p.first == semLbl; });
        if (semIt == semesters.end()) {
            semesters.push_back({ semLbl, {} });
            semIt = semesters.end() - 1;
        }

        semIt->second.push_back({ cname, credits, grade });
    }

    // Reconstituer les objets du domaine dans l'ordre d'insertion
    for (const auto& id : studentOrder) {
        const auto& [sname, semesters] = studentData[id];
        Student student(id, sname);

        for (const auto& [semLabel, courses] : semesters) {
            Semester semester(semLabel);
            for (const auto& [cname, credits, grade] : courses) {
                semester.addCourse(Course(cname, credits, grade));
            }
            student.addSemester(semester);
        }

        manager.addStudent(student);
    }
}

} // namespace cgpa
