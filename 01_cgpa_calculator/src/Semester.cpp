#include "cgpa/Semester.hpp"
#include <iostream>
#include <iomanip>

namespace cgpa {

Semester::Semester(std::string label) : label_(std::move(label)) {}

void Semester::addCourse(const Course& c) {
    courses_.push_back(c);
}

double Semester::totalCredits() const {
    double sum = 0;
    for (const auto& c : courses_) sum += c.getCreditHours();
    return sum;
}

double Semester::totalQualityPoints() const {
    double sum = 0;
    for (const auto& c : courses_) sum += c.getQualityPoints();
    return sum;
}

double Semester::gpa() const {
    double credits = totalCredits();
    return credits == 0 ? 0.0 : totalQualityPoints() / credits;
}

const std::string& Semester::getLabel() const { return label_; }
const std::vector<Course>& Semester::getCourses() const { return courses_; }

void Semester::print() const {
    std::cout << "\n--- " << label_ << " ---\n";
    std::cout << std::left << std::setw(22) << "Cours" << std::setw(10) << "Credits"
              << std::setw(8) << "Note" << std::setw(8) << "Points" << "\n";
    for (const auto& c : courses_) c.print();
    std::cout << "GPA du semestre : " << std::fixed << std::setprecision(2) << gpa() << "\n";
}

} // namespace cgpa
