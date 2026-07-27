#pragma once
#include <string>
#include "cgpa/StudentManager.hpp"

namespace cgpa {

// Sauvegarde / charge un StudentManager en format CSV.
// Format : student_id,student_name,semester,course_name,credits,letter_grade
// Une ligne par cours — les étudiants et semestres sont reconstitués au chargement
// par regroupement sur (student_id, semester_label).
class FileManager {
public:
    // Lève std::runtime_error en cas d'erreur d'ouverture ou d'écriture.
    static void save(const StudentManager& manager, const std::string& filepath);

    // Lève std::runtime_error en cas d'erreur de lecture ou de format invalide.
    static void load(StudentManager& manager, const std::string& filepath);

private:
    static std::vector<std::string> splitCsv(const std::string& line, char delim = ',');
};

} // namespace cgpa
