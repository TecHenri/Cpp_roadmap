#pragma once
#include <string>
#include <vector>
#include "cgpa/Course.hpp"

namespace cgpa {

// Représente un semestre : un ensemble de cours, calcule le GPA du semestre.
class Semester {
public:
    explicit Semester(std::string label);

    void addCourse(const Course& c);

    double totalCredits() const;
    double totalQualityPoints() const;
    double gpa() const;

    const std::string& getLabel() const;
    const std::vector<Course>& getCourses() const;

    void print() const;

private:
    std::string label_;
    std::vector<Course> courses_;
};

} // namespace cgpa
