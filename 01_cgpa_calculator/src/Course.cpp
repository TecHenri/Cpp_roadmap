#include "cgpa/Course.hpp"
#include "cgpa/GradeScale.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace cgpa {

Course::Course(std::string name, double creditHours, std::string letterGrade)
    : name_(std::move(name)),
      creditHours_(creditHours),
      letterGrade_(std::move(letterGrade)) {
    if (creditHours_ <= 0) {
        throw std::invalid_argument("Le nombre de credits doit etre strictement positif.");
    }
    gradePoint_ = GradeScale::toGradePoint(letterGrade_);
}

double Course::getCreditHours() const { return creditHours_; }
double Course::getGradePoint() const { return gradePoint_; }
double Course::getQualityPoints() const { return creditHours_ * gradePoint_; }
const std::string& Course::getName() const { return name_; }
const std::string& Course::getLetterGrade() const { return letterGrade_; }

void Course::print() const {
    std::cout << std::left << std::setw(22) << name_
              << std::setw(10) << creditHours_
              << std::setw(8) << letterGrade_
              << std::setw(8) << gradePoint_ << "\n";
}

} // namespace cgpa
