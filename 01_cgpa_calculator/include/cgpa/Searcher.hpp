#pragma once
#include <vector>
#include <string>
#include "cgpa/Student.hpp"

namespace cgpa {

// Algorithmes de recherche écrits à la main.
class Searcher {
public:
    // Recherche linéaire par ID - O(n), fonctionne sur une liste non triée
    static const Student* linearSearchById(const std::vector<Student>& students,
                                            const std::string& id);

    // Recherche binaire par ID - O(log n), nécessite une liste triée par ID
    static const Student* binarySearchById(const std::vector<Student>& sortedStudents,
                                            const std::string& id);
};

} // namespace cgpa
