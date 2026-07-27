#pragma once
#include <vector>
#include <string>
#include "cgpa/Student.hpp"

namespace cgpa {

// [V2 - pas encore implémenté]
// Gère la collection de tous les étudiants : ajout, suppression, accès.
// Sert de point d'entrée unique pour Sorter, Searcher et FileManager.
class StudentManager {
public:
    void addStudent(const Student& s);
    bool removeStudent(const std::string& id);

    std::vector<Student>& getAll();
    const std::vector<Student>& getAll() const;

private:
    std::vector<Student> students_;
};

} // namespace cgpa
