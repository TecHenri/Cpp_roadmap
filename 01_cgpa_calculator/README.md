# C++ Learning Series

[![CI](https://github.com/TecHenri/Cpp_roadmap/01_cgpa_calculator/actions/workflows/build.yml/badge.svg)](https://github.com/TecHenri/Cpp_roadmap/01_cgpa_calculator/actions/workflows/build.yml)

Une série de projets C++ de complexité croissante pour maîtriser le C++ moderne (C++17), l'architecture logicielle, les algorithmes, et le développement full-stack.

---

## Projets

| # | Project | Concepts | Status |
|---|---------|----------|--------|
| 01 | [CGPA Calculator](./01_cgpa_calculator/) | OOP, Algorithms, REST API, SQLite, React | ✅ Complete |

---

## Stack utilisée

- **Language** : C++17
- **Build** : CMake (cross-platform Windows/Linux)
- **HTTP** : [cpp-httplib](https://github.com/yhirose/cpp-httplib) (header-only)
- **JSON** : [nlohmann/json](https://github.com/nlohmann/json) (header-only)
- **Database** : SQLite (amalgamation)
- **Frontend** : React + Vite

---

## Comment compiler un projet

```bash
cd <project_folder>
mkdir build && cd build
cmake ..
make          # Linux/Mac
# or open CMakeLists.txt in Visual Studio on Windows
```
