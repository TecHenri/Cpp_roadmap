#include "cgpa/Menu.hpp"
#include "cgpa/Course.hpp"
#include "cgpa/Semester.hpp"
#include "cgpa/Student.hpp"
#include "cgpa/Sorter.hpp"
#include "cgpa/Searcher.hpp"
#include "cgpa/FileManager.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

namespace cgpa {

// ----------------------------------------------------------------
// Constructeur
// ----------------------------------------------------------------

Menu::Menu(StudentManager& manager) : manager_(manager) {}

// ----------------------------------------------------------------
// Helpers de saisie
// ----------------------------------------------------------------

void Menu::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Menu::pauseScreen() {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string Menu::readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

int Menu::readInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Saisie invalide. Entrez un entier entre "
                  << min << " et " << max << ".\n";
    }
}

double Menu::readDouble(const std::string& prompt, double min) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Saisie invalide. Entrez un nombre >= " << min << ".\n";
    }
}

// ----------------------------------------------------------------
// Saisie d'un semestre complet
// ----------------------------------------------------------------

Semester Menu::inputSemester() {
    std::string label = readLine("  Label du semestre (ex: Semestre 1) : ");
    Semester sem(label);

    int nbCours = readInt("  Nombre de cours : ", 1, 20);
    for (int i = 0; i < nbCours; ++i) {
        std::cout << "  -- Cours " << (i + 1) << " --\n";
        std::string nom    = readLine("    Nom du cours : ");
        double credits     = readDouble("    Credits (ex: 3) : ", 0.5);
        std::string note;

        while (true) {
            note = readLine("    Note (A+/A/A-/B+/B/B-/C+/C/C-/D+/D/F) : ");
            try {
                sem.addCourse(Course(nom, credits, note));
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << "    Erreur : " << e.what() << "\n";
            }
        }
    }
    return sem;
}

// ----------------------------------------------------------------
// Saisie d'un etudiant complet
// ----------------------------------------------------------------

Student Menu::inputStudent() {
    std::string id   = readLine("  ID etudiant (ex: S010) : ");
    std::string nom  = readLine("  Nom complet : ");
    Student etu(id, nom);

    int nbSem = readInt("  Nombre de semestres : ", 1, 10);
    for (int i = 0; i < nbSem; ++i) {
        std::cout << "\n  === Semestre " << (i + 1) << " ===\n";
        etu.addSemester(inputSemester());
    }
    return etu;
}

// ----------------------------------------------------------------
// Affichage de la liste des etudiants (vue compacte)
// ----------------------------------------------------------------

void Menu::handleListStudents() {
    clearScreen();
    const auto& students = manager_.getAll();
    if (students.empty()) {
        std::cout << "Aucun etudiant enregistre.\n";
        pauseScreen();
        return;
    }

    std::cout << "=== Liste des etudiants (" << students.size() << ") ===\n\n";
    std::cout << std::left << std::setw(8)  << "ID"
              << std::setw(25) << "Nom"
              << std::setw(12) << "Semestres"
              << "CGPA\n";
    std::cout << std::string(55, '-') << "\n";

    for (const auto& etu : students) {
        std::cout << std::left << std::setw(8)  << etu.getId()
                  << std::setw(25) << etu.getName()
                  << std::setw(12) << etu.getSemesters().size()
                  << std::fixed << std::setprecision(2) << etu.cgpa() << "\n";
    }

    std::cout << "\n[D] Voir le detail d'un etudiant | [Entree] Retour\n";
    std::string choix = readLine("> ");
    if (choix == "D" || choix == "d") {
        std::string id = readLine("ID de l'etudiant : ");
        const Student* etu = Searcher::linearSearchById(students, id);
        if (etu) {
            std::cout << "\n";
            etu->printReport();
        } else {
            std::cout << "Etudiant non trouve.\n";
        }
        pauseScreen();
    }
}

// ----------------------------------------------------------------
// Ajout d'un etudiant
// ----------------------------------------------------------------

void Menu::handleAddStudent() {
    clearScreen();
    std::cout << "=== Ajouter un etudiant ===\n\n";
    try {
        Student etu = inputStudent();
        manager_.addStudent(etu);
        std::cout << "\nEtudiant \"" << etu.getName() << "\" ajoute avec succes.\n";
        std::cout << "CGPA : " << std::fixed << std::setprecision(2) << etu.cgpa() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Erreur : " << e.what() << "\n";
    }
    pauseScreen();
}

// ----------------------------------------------------------------
// Suppression d'un etudiant
// ----------------------------------------------------------------

void Menu::handleRemoveStudent() {
    clearScreen();
    std::cout << "=== Supprimer un etudiant ===\n\n";
    std::string id = readLine("ID de l'etudiant a supprimer : ");
    if (manager_.removeStudent(id)) {
        std::cout << "Etudiant " << id << " supprime.\n";
    } else {
        std::cout << "ID introuvable : " << id << "\n";
    }
    pauseScreen();
}

// ----------------------------------------------------------------
// Recherche
// ----------------------------------------------------------------

void Menu::handleSearch() {
    clearScreen();
    std::cout << "=== Recherche ===\n\n";
    std::cout << "1. Recherche lineaire (liste non triee)\n";
    std::cout << "2. Recherche binaire  (trie par ID)\n";
    int choix = readInt("> ", 1, 2);
    std::string id = readLine("ID recherche : ");

    const Student* result = nullptr;

    if (choix == 1) {
        result = Searcher::linearSearchById(manager_.getAll(), id);
    } else {
        // La recherche binaire necessite une liste triee par ID
        Sorter::quickSortById(manager_.getAll());
        result = Searcher::binarySearchById(manager_.getAll(), id);
    }

    if (result) {
        std::cout << "\n";
        result->printReport();
    } else {
        std::cout << "Aucun etudiant avec l'ID \"" << id << "\".\n";
    }
    pauseScreen();
}

// ----------------------------------------------------------------
// Tri
// ----------------------------------------------------------------

void Menu::handleSort() {
    clearScreen();
    std::cout << "=== Trier les etudiants ===\n\n";
    std::cout << "1. Par CGPA decroissant\n";
    std::cout << "2. Par nom alphabetique\n";
    std::cout << "3. Par ID croissant\n";
    int choix = readInt("> ", 1, 3);

    switch (choix) {
        case 1: Sorter::quickSortByCgpa(manager_.getAll()); std::cout << "Trie par CGPA.\n"; break;
        case 2: Sorter::quickSortByName(manager_.getAll()); std::cout << "Trie par nom.\n";  break;
        case 3: Sorter::quickSortById(manager_.getAll());   std::cout << "Trie par ID.\n";   break;
    }

    std::cout << "\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(25) << "Nom"
              << "CGPA\n";
    std::cout << std::string(40, '-') << "\n";
    for (const auto& etu : manager_.getAll()) {
        std::cout << std::left << std::setw(8) << etu.getId()
                  << std::setw(25) << etu.getName()
                  << std::fixed << std::setprecision(2) << etu.cgpa() << "\n";
    }
    pauseScreen();
}

// ----------------------------------------------------------------
// Sauvegarde / Chargement
// ----------------------------------------------------------------

void Menu::handleSaveLoad() {
    clearScreen();
    std::cout << "=== Sauvegarde / Chargement ===\n\n";
    std::cout << "1. Sauvegarder (CSV)\n";
    std::cout << "2. Charger     (CSV)\n";
    int choix = readInt("> ", 1, 2);

    std::string path = readLine("Chemin du fichier (ex: data/students.csv) : ");

    try {
        if (choix == 1) {
            FileManager::save(manager_, path);
            std::cout << "Sauvegarde OK -> " << path << "\n";
        } else {
            FileManager::load(manager_, path);
            std::cout << "Chargement OK : "
                      << manager_.getAll().size() << " etudiants.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Erreur : " << e.what() << "\n";
    }
    pauseScreen();
}

// ----------------------------------------------------------------
// Menu principal
// ----------------------------------------------------------------

void Menu::showMainMenu() {
    clearScreen();
    std::cout << "============================================\n";
    std::cout << "         CGPA Calculator v1.0\n";
    std::cout << "============================================\n";
    std::cout << "  Etudiants enregistres : "
              << manager_.getAll().size() << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "  1. Lister les etudiants\n";
    std::cout << "  2. Ajouter un etudiant\n";
    std::cout << "  3. Supprimer un etudiant\n";
    std::cout << "  4. Rechercher\n";
    std::cout << "  5. Trier\n";
    std::cout << "  6. Sauvegarder / Charger\n";
    std::cout << "  0. Quitter\n";
    std::cout << "--------------------------------------------\n";
}

void Menu::run() {
    while (true) {
        showMainMenu();
        int choix = readInt("> ", 0, 6);

        switch (choix) {
            case 1: handleListStudents(); break;
            case 2: handleAddStudent();   break;
            case 3: handleRemoveStudent();break;
            case 4: handleSearch();       break;
            case 5: handleSort();         break;
            case 6: handleSaveLoad();     break;
            case 0:
                clearScreen();
                std::cout << "Au revoir.\n";
                return;
        }
    }
}

} // namespace cgpa
