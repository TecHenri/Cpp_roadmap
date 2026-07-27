#pragma once
#include <string>
#include "cgpa/StudentManager.hpp"
#include "cgpa/DatabaseManager.hpp"

namespace cgpa {

class ApiServer {
public:
    // Reçoit StudentManager ET DatabaseManager pour les opérations live
    ApiServer(StudentManager& manager, DatabaseManager& db, int port = 8080);
    void run();

private:
    StudentManager&  manager_;
    DatabaseManager& db_;
    int              port_;

    void setupRoutes();
};

} // namespace cgpa
