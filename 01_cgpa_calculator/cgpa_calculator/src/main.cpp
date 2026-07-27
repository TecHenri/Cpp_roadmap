#include "cgpa/StudentManager.hpp"
#include "cgpa/DatabaseManager.hpp"
#include "cgpa/ApiServer.hpp"
#include <iostream>
#include <memory>

static const std::string DB_PATH = std::string(CGPA_DATA_DIR) + "/cgpa.db";

int main() {
    try {
        cgpa::StudentManager  manager;
        cgpa::DatabaseManager db(DB_PATH);

        // Chargement auto depuis SQLite
        db.load(manager);
        std::cout << manager.getAll().size()
                  << " etudiant(s) charge(s) depuis " << DB_PATH << "\n";

        // Lancement API REST — on passe aussi db pour les opérations live
        cgpa::ApiServer server(manager, db);
        server.run(); // bloquant

        // Sauvegarde à l'arrêt (Ctrl+C)
        db.save(manager);
        std::cout << "Base de donnees sauvegardee.\n";

    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale : " << e.what() << "\n";
        return 1;
    }
    return 0;
}
