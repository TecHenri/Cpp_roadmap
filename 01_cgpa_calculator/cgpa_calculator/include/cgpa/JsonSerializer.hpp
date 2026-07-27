#pragma once
#include "cgpa/Student.hpp"
#include "cgpa/Semester.hpp"
#include "cgpa/Course.hpp"
#include <nlohmann/json.hpp>

namespace cgpa {

using json = nlohmann::json;

// Course → JSON
inline json courseToJson(const Course& c) {
    return {
        {"name",        c.getName()},
        {"credits",     c.getCreditHours()},
        {"letterGrade", c.getLetterGrade()},
        {"gradePoint",  c.getGradePoint()},
        {"qualityPts",  c.getQualityPoints()}
    };
}

// Semester → JSON
inline json semesterToJson(const Semester& s) {
    json courses = json::array();
    for (const auto& c : s.getCourses()) {
        courses.push_back(courseToJson(c));
    }
    return {
        {"label",         s.getLabel()},
        {"gpa",           s.gpa()},
        {"totalCredits",  s.totalCredits()},
        {"courses",       courses}
    };
}

// Student → JSON
inline json studentToJson(const Student& s) {
    json semesters = json::array();
    for (const auto& sem : s.getSemesters()) {
        semesters.push_back(semesterToJson(sem));
    }
    return {
        {"id",        s.getId()},
        {"name",      s.getName()},
        {"cgpa",      s.cgpa()},
        {"semesters", semesters}
    };
}

} // namespace cgpa
