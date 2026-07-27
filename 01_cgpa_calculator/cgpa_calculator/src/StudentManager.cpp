#include "cgpa/StudentManager.hpp"
#include <algorithm>

namespace cgpa {

void StudentManager::addStudent(const Student& s) {
    students_.push_back(s);
}

bool StudentManager::removeStudent(const std::string& id) {
    auto it = std::find_if(students_.begin(), students_.end(),
                            [&id](const Student& s) { return s.getId() == id; });
    if (it == students_.end()) {
        return false;
    }
    students_.erase(it);
    return true;
}

std::vector<Student>& StudentManager::getAll() { return students_; }
const std::vector<Student>& StudentManager::getAll() const { return students_; }

} // namespace cgpa
