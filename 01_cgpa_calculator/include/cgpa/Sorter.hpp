#pragma once
#include <vector>
#include "cgpa/Student.hpp"

namespace cgpa {

// Algorithmes de tri écrits à la main (volontairement, pas std::sort)
// pour pratiquer la complexité algorithmique.
class Sorter {
public:
    // Tri par CGPA décroissant - quicksort, O(n log n) en moyenne
    static void quickSortByCgpa(std::vector<Student>& students);

    // Tri alphabétique par nom
    static void quickSortByName(std::vector<Student>& students);

    // Tri par ID croissant - prérequis pour binarySearchById
    static void quickSortById(std::vector<Student>& students);
};

} // namespace cgpa
