#include "cgpa/Student.hpp"
#include <iostream>
#include <iomanip>

namespace cgpa {

Student::Student(std::string id, std::string name)
    : id_(std::move(id)), name_(std::move(name)) {}

void Student::addSemester(const Semester& s) {
    semesters_.push_back(s);
}

double Student::cgpa() const {
    double totalCredits = 0, totalQuality = 0;
    for (const auto& s : semesters_) {
        totalCredits += s.totalCredits();
        totalQuality += s.totalQualityPoints();
    }
    return totalCredits == 0 ? 0.0 : totalQuality / totalCredits;
}

const std::string& Student::getId() const { return id_; }
const std::string& Student::getName() const { return name_; }
const std::vector<Semester>& Student::getSemesters() const { return semesters_; }

void Student::printReport() const {
    std::cout << "==========================================\n";
    std::cout << "Etudiant : " << name_ << " (ID: " << id_ << ")\n";
    for (const auto& s : semesters_) s.print();
    std::cout << "------------------------------------------\n";
    std::cout << "CGPA global : " << std::fixed << std::setprecision(2) << cgpa() << "\n";
    std::cout << "==========================================\n";
}

} // namespace cgpa
