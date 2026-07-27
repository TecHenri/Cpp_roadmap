#include "cgpa/ApiServer.hpp"
#include "cgpa/JsonSerializer.hpp"
#include "cgpa/Course.hpp"
#include "cgpa/Semester.hpp"
#include "cgpa/Student.hpp"
#include "cgpa/Sorter.hpp"
#include "cgpa/Searcher.hpp"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

namespace cgpa {

using json = nlohmann::json;

ApiServer::ApiServer(StudentManager& manager, DatabaseManager& db, int port)
    : manager_(manager), db_(db), port_(port) {}

static void setCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

static void jsonResponse(httplib::Response& res, const json& body, int status = 200) {
    setCors(res);
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}

static void errorResponse(httplib::Response& res, const std::string& msg, int status = 400) {
    jsonResponse(res, {{"error", msg}}, status);
}

static Student studentFromJson(const json& j) {
    Student stu(j.at("id").get<std::string>(),
                j.at("name").get<std::string>());
    for (const auto& semJ : j.at("semesters")) {
        Semester sem(semJ.at("label").get<std::string>());
        for (const auto& cJ : semJ.at("courses")) {
            sem.addCourse(Course(
                cJ.at("name").get<std::string>(),
                cJ.at("credits").get<double>(),
                cJ.at("letterGrade").get<std::string>()
            ));
        }
        stu.addSemester(sem);
    }
    return stu;
}

void ApiServer::setupRoutes() {
    auto svr = std::make_shared<httplib::Server>();

    svr->Options(".*", [](const httplib::Request&, httplib::Response& res) {
        setCors(res);
        res.status = 204;
    });

    // GET /api/students
    svr->Get("/api/students", [this](const httplib::Request&, httplib::Response& res) {
        json arr = json::array();
        for (const auto& s : manager_.getAll()) arr.push_back(studentToJson(s));
        jsonResponse(res, arr);
    });

    // GET /api/students/sort?by=cgpa|name|id
    svr->Get("/api/students/sort", [this](const httplib::Request& req, httplib::Response& res) {
        std::string by = req.has_param("by") ? req.get_param_value("by") : "cgpa";
        if      (by == "name") Sorter::quickSortByName(manager_.getAll());
        else if (by == "id")   Sorter::quickSortById(manager_.getAll());
        else                   Sorter::quickSortByCgpa(manager_.getAll());
        json arr = json::array();
        for (const auto& s : manager_.getAll()) arr.push_back(studentToJson(s));
        jsonResponse(res, arr);
    });

    // GET /api/students/search?id=XXX
    svr->Get("/api/students/search", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("id")) { errorResponse(res, "Parametre 'id' manquant"); return; }
        std::string id = req.get_param_value("id");
        const Student* found = Searcher::linearSearchById(manager_.getAll(), id);
        if (found) jsonResponse(res, studentToJson(*found));
        else       errorResponse(res, "Etudiant non trouve : " + id, 404);
    });

    // GET /api/students/:id
    svr->Get(R"(/api/students/([^/]+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        const Student* found = Searcher::linearSearchById(manager_.getAll(), id);
        if (found) jsonResponse(res, studentToJson(*found));
        else       errorResponse(res, "Etudiant non trouve : " + id, 404);
    });

    // POST /api/students — écrit directement en base SQLite
    svr->Post("/api/students", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            Student stu = studentFromJson(body);
            if (Searcher::linearSearchById(manager_.getAll(), stu.getId())) {
                errorResponse(res, "ID deja existant : " + stu.getId(), 409);
                return;
            }
            db_.insertStudent(stu);      // ← SQLite live
            manager_.addStudent(stu);    // ← mémoire en sync
            jsonResponse(res, studentToJson(stu), 201);
        } catch (const json::exception& e) {
            errorResponse(res, std::string("JSON invalide : ") + e.what());
        } catch (const std::exception& e) {
            errorResponse(res, e.what());
        }
    });

    // DELETE /api/students/:id — supprime en base ET en mémoire
    svr->Delete(R"(/api/students/([^/]+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string id = req.matches[1];
        if (db_.deleteStudent(id)) {     // ← SQLite live
            manager_.removeStudent(id);  // ← mémoire en sync
            jsonResponse(res, {{"message", "Etudiant " + id + " supprime"}});
        } else {
            errorResponse(res, "Etudiant non trouve : " + id, 404);
        }
    });

    // GET /api/stats
    svr->Get("/api/stats", [this](const httplib::Request&, httplib::Response& res) {
        const auto& students = manager_.getAll();
        double total = 0;
        for (const auto& s : students) total += s.cgpa();
        double avg = students.empty() ? 0 : total / students.size();
        jsonResponse(res, {{"totalStudents", students.size()}, {"averageCgpa", avg}});
    });

    std::cout << "API demarree sur http://localhost:" << port_ << "\n";
    std::cout << "Base SQLite : " << "\n";
    svr->listen("0.0.0.0", port_);
}

void ApiServer::run() { setupRoutes(); }

} // namespace cgpa
