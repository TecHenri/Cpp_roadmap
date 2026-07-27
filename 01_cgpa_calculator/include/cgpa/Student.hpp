#pragma once
#include <string>
#include <vector>
#include "cgpa/Semester.hpp"

namespace cgpa {

// Représente un étudiant : identité + parcours (plusieurs semestres).
// Calcule le CGPA global à partir de tous les semestres.
class Student {
public:
    Student(std::string id, std::string name);

    void addSemester(const Semester& s);

    double cgpa() const;

    const std::string& getId() const;
    const std::string& getName() const;
    const std::vector<Semester>& getSemesters() const;

    void printReport() const;

private:
    std::string id_;
    std::string name_;
    std::vector<Semester> semesters_;
};

} // namespace cgpa
