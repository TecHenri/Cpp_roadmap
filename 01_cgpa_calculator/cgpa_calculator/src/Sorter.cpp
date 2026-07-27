#include "cgpa/Sorter.hpp"
#include <utility>

namespace cgpa {

namespace {

// --- Tri par CGPA decroissant ---

int partitionByCgpaDesc(std::vector<Student>& students, int low, int high) {
    double pivot = students[high].cgpa();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (students[j].cgpa() > pivot) { // ">" car on veut du plus grand au plus petit
            ++i;
            std::swap(students[i], students[j]);
        }
    }
    std::swap(students[i + 1], students[high]);
    return i + 1;
}

void quickSortByCgpaRange(std::vector<Student>& students, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionByCgpaDesc(students, low, high);
        quickSortByCgpaRange(students, low, pivotIndex - 1);
        quickSortByCgpaRange(students, pivotIndex + 1, high);
    }
}

// --- Tri alphabetique par nom ---

int partitionByName(std::vector<Student>& students, int low, int high) {
    const std::string pivot = students[high].getName();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (students[j].getName() < pivot) {
            ++i;
            std::swap(students[i], students[j]);
        }
    }
    std::swap(students[i + 1], students[high]);
    return i + 1;
}

void quickSortByNameRange(std::vector<Student>& students, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionByName(students, low, high);
        quickSortByNameRange(students, low, pivotIndex - 1);
        quickSortByNameRange(students, pivotIndex + 1, high);
    }
}

// --- Tri par ID croissant (prerequis recherche binaire) ---

int partitionById(std::vector<Student>& students, int low, int high) {
    const std::string pivot = students[high].getId();
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (students[j].getId() < pivot) {
            ++i;
            std::swap(students[i], students[j]);
        }
    }
    std::swap(students[i + 1], students[high]);
    return i + 1;
}

void quickSortByIdRange(std::vector<Student>& students, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionById(students, low, high);
        quickSortByIdRange(students, low, pivotIndex - 1);
        quickSortByIdRange(students, pivotIndex + 1, high);
    }
}

} // namespace

void Sorter::quickSortByCgpa(std::vector<Student>& students) {
    if (!students.empty()) {
        quickSortByCgpaRange(students, 0, static_cast<int>(students.size()) - 1);
    }
}

void Sorter::quickSortByName(std::vector<Student>& students) {
    if (!students.empty()) {
        quickSortByNameRange(students, 0, static_cast<int>(students.size()) - 1);
    }
}

void Sorter::quickSortById(std::vector<Student>& students) {
    if (!students.empty()) {
        quickSortByIdRange(students, 0, static_cast<int>(students.size()) - 1);
    }
}

} // namespace cgpa
