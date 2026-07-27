#pragma once
#include <string>
#include "cgpa/StudentManager.hpp"

namespace cgpa {

// Boucle de menu console interactive qui relie tous les modules :
// StudentManager, Sorter, Searcher, FileManager.
class Menu {
public:
    explicit Menu(StudentManager& manager);
    void run();

private:
    StudentManager& manager_;

    // Sous-menus principaux
    void showMainMenu();
    void handleListStudents();
    void handleAddStudent();
    void handleRemoveStudent();
    void handleSearch();
    void handleSort();
    void handleSaveLoad();

    // Helpers de saisie
    Student       inputStudent();
    Semester      inputSemester();
    static int    readInt(const std::string& prompt, int min, int max);
    static double readDouble(const std::string& prompt, double min);
    static std::string readLine(const std::string& prompt);
    static void   clearScreen();
    static void   pauseScreen();
};

} // namespace cgpa
