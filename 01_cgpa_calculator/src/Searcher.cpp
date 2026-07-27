#include "cgpa/Searcher.hpp"

namespace cgpa {

const Student* Searcher::linearSearchById(const std::vector<Student>& students,
                                           const std::string& id) {
    for (const auto& s : students) {
        if (s.getId() == id) {
            return &s;
        }
    }
    return nullptr;
}

const Student* Searcher::binarySearchById(const std::vector<Student>& sortedStudents,
                                           const std::string& id) {
    int low = 0;
    int high = static_cast<int>(sortedStudents.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        const std::string& midId = sortedStudents[mid].getId();

        if (midId == id) {
            return &sortedStudents[mid];
        } else if (midId < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return nullptr;
}

} // namespace cgpa
