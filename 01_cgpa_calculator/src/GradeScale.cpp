#include "cgpa/GradeScale.hpp"
#include <map>
#include <stdexcept>

namespace cgpa {

double GradeScale::toGradePoint(const std::string& letter) {
    static const std::map<std::string, double> scale = {
        {"A+", 4.0}, {"A", 4.0}, {"A-", 3.7},
        {"B+", 3.3}, {"B", 3.0}, {"B-", 2.7},
        {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7},
        {"D+", 1.3}, {"D", 1.0},
        {"F", 0.0}
    };

    auto it = scale.find(letter);
    if (it == scale.end()) {
        throw std::invalid_argument("Lettre de note invalide : '" + letter + "'");
    }
    return it->second;
}

} // namespace cgpa
