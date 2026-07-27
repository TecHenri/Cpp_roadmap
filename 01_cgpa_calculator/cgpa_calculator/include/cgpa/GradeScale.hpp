#pragma once
#include <string>

namespace cgpa {

// Convertit une note alphabétique en grade point numérique.
// Isolé dans sa propre classe pour pouvoir changer d'échelle
// (sur 4.0, sur 5.0, numérique...) sans toucher au reste du code.
class GradeScale {
public:
    static double toGradePoint(const std::string& letter);
};

} // namespace cgpa
