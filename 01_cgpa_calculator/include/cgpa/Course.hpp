#pragma once
#include <string>

namespace cgpa {

// Représente un cours suivi par un étudiant durant un semestre.
class Course {
public:
    Course(std::string name, double creditHours, std::string letterGrade);

    double getCreditHours() const;
    double getGradePoint() const;
    double getQualityPoints() const; // creditHours * gradePoint
    const std::string& getName() const;
    const std::string& getLetterGrade() const;

    void print() const;

private:
    std::string name_;
    double creditHours_;
    std::string letterGrade_;
    double gradePoint_;
};

} // namespace cgpa
