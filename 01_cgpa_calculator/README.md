# 01 — CGPA Calculator

> **C++ · CMake · REST API · SQLite · React**

---

### Description

Calculateur de moyenne cumulative (CGPA) complet, avec backend C++ exposant une API REST et interface web React. Le projet couvre l'ensemble de la stack d'une application réelle : modèle de données, algorithmes, persistance, API, et frontend.

### Concepts pratiqués

| Catégorie | Détails |
|-----------|---------|
| **POO C++17** | Classes, namespaces, constructeurs move, exceptions |
| **Architecture** | Séparation header/implémentation, CMake multi-plateforme |
| **Algorithmes** | Quicksort manuel (3 critères), recherche binaire et linéaire |
| **Persistance** | SQLite embarqué (amalgamation), transactions atomiques, clés étrangères |
| **API REST** | Serveur HTTP C++ (cpp-httplib), JSON (nlohmann), CORS |
| **Frontend** | React, Vite, appels API avec Axios, composants dynamiques |

### Architecture

```
┌─────────────────────────┐
│    Frontend React        │  http://localhost:5173
│  Dashboard / Étudiants  │
│  Ajouter / Rechercher   │
└────────────┬────────────┘
             │ HTTP REST (JSON)
             │ GET  /api/students
             │ POST /api/students
             │ DELETE /api/students/:id
             │ GET  /api/stats
┌────────────┴────────────┐
│  Backend C++ (port 8080) │
│  StudentManager          │
│  Sorter / Searcher       │
│  DatabaseManager         │
│  ApiServer               │
└────────────┬────────────┘
             │
       data/cgpa.db  (SQLite)
```

### Modèle de données

```
Student (id, name)
  └── Semester (label)
        └── Course (name, credits, letterGrade)

CGPA = Σ(credits × gradePoint) / Σ(credits)
```

### Schéma SQLite

```sql
TABLE students   (id TEXT PK, name TEXT)
TABLE semesters  (id INT PK AUTO, student_id FK, label TEXT, position INT)
TABLE courses    (id INT PK AUTO, semester_id FK, name TEXT,
                  credits REAL, letter_grade TEXT, position INT)
```

Les suppressions sont propagées en cascade (`ON DELETE CASCADE`).

### Structure du projet

```
01_cgpa_calculator/
├── CMakeLists.txt
├── include/cgpa/          ← headers publics (.hpp)
│   ├── Course.hpp
│   ├── Semester.hpp
│   ├── Student.hpp
│   ├── GradeScale.hpp
│   ├── StudentManager.hpp
│   ├── Sorter.hpp
│   ├── Searcher.hpp
│   ├── FileManager.hpp
│   ├── DatabaseManager.hpp
│   ├── JsonSerializer.hpp
│   ├── ApiServer.hpp
│   └── Menu.hpp
├── src/                   ← implémentations (.cpp)
├── vendor/
│   ├── sqlite/            ← SQLite amalgamation
│   ├── httplib/           ← cpp-httplib (header-only)
│   └── nlohmann/          ← JSON (header-only)
├── data/
│   └── cgpa.db            ← base SQLite (8 étudiants de test)
└── cgpa-frontend/         ← application React
    ├── src/
    │   ├── api.js
    │   ├── App.jsx
    │   ├── pages/
    │   │   ├── Dashboard.jsx
    │   │   ├── Students.jsx
    │   │   ├── AddStudent.jsx
    │   │   └── Search.jsx
    │   └── components/
    │       └── BackendStatus.jsx
    └── package.json
```

### Lancer le projet

**Prérequis** : Visual Studio 2022, Node.js ≥ 18

```bash
# 1. Backend C++
#    Ouvrir CMakeLists.txt dans Visual Studio
#    Ctrl+Shift+B  puis  F5
#    La console affiche : "API demarrée sur http://localhost:8080"

# 2. Frontend React
cd cgpa-frontend
npm install
npm run dev
#    Ouvrir http://localhost:5173
```

La pastille verte dans la navbar confirme que le backend est connecté.

### Données de test

Le fichier `data/cgpa.db` contient 8 étudiants avec des profils variés
(CGPA de 1.51 à 4.00, 2 à 4 semestres chacun).

---

## 🇬🇧 English

### Description

A full-featured Cumulative Grade Point Average (CGPA) calculator with a C++ REST API backend and a React web interface. The project covers the complete stack of a real-world application: data modelling, algorithms, persistence, API design, and frontend.

### Concepts covered

| Category | Details |
|----------|---------|
| **Modern C++17** | Classes, namespaces, move semantics, exceptions |
| **Architecture** | Header/implementation separation, cross-platform CMake |
| **Algorithms** | Manual quicksort (3 keys), binary and linear search |
| **Persistence** | Embedded SQLite (amalgamation), atomic transactions, foreign keys |
| **REST API** | C++ HTTP server (cpp-httplib), JSON (nlohmann), CORS |
| **Frontend** | React, Vite, Axios API calls, dynamic components |

### How to run

**Requirements** : Visual Studio 2022 (Windows) or GCC/Clang (Linux/Mac), Node.js ≥ 18

```bash
# 1. C++ Backend
#    Open CMakeLists.txt in Visual Studio
#    Ctrl+Shift+B  then  F5
#    Console prints: "API demarrée sur http://localhost:8080"

# 2. React Frontend
cd cgpa-frontend
npm install
npm run dev
#    Open http://localhost:5173
```

### REST API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| `GET` | `/api/students` | List all students |
| `GET` | `/api/students/:id` | Get one student |
| `GET` | `/api/students/sort?by=cgpa\|name\|id` | Sorted list |
| `GET` | `/api/students/search?id=XXX` | Search by ID |
| `POST` | `/api/students` | Add a student |
| `DELETE` | `/api/students/:id` | Remove a student |
| `GET` | `/api/stats` | Global statistics |

### Grade Scale (4.0 system)

| Letter | Points | Letter | Points |
|--------|--------|--------|--------|
| A+ / A | 4.0 | C+ | 2.3 |
| A- | 3.7 | C | 2.0 |
| B+ | 3.3 | C- | 1.7 |
| B | 3.0 | D+ | 1.3 |
| B- | 2.7 | D | 1.0 |
| | | F | 0.0 |

### Dependencies (all vendored — no install required)

| Library | License |
|---------|---------|
| [cpp-httplib](https://github.com/yhirose/cpp-httplib) | MIT |
| [nlohmann/json](https://github.com/nlohmann/json) | MIT |
| [SQLite](https://sqlite.org) | Public Domain |

---

## License

MIT
