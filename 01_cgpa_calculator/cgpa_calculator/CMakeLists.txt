cmake_minimum_required(VERSION 3.10)
project(CGPACalculator LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor
)

add_compile_definitions(CGPA_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

find_package(Threads REQUIRED)

# SQLite compilé sans nos flags stricts (warnings internes à SQLite)
add_library(sqlite3_lib STATIC vendor/sqlite/sqlite3.c)
target_include_directories(sqlite3_lib PUBLIC ${PROJECT_SOURCE_DIR}/vendor/sqlite)

# Notre code avec tous les warnings activés
add_executable(cgpa_calculator
    src/main.cpp
    src/GradeScale.cpp
    src/Course.cpp
    src/Semester.cpp
    src/Student.cpp
    src/StudentManager.cpp
    src/Sorter.cpp
    src/Searcher.cpp
    src/FileManager.cpp
    src/DatabaseManager.cpp
    src/ApiServer.cpp
)

if(MSVC)
    target_compile_options(cgpa_calculator PRIVATE /W4 /WX-)
else()
    target_compile_options(cgpa_calculator PRIVATE -Wall -Wextra)
endif()

target_link_libraries(cgpa_calculator PRIVATE sqlite3_lib Threads::Threads)

if(WIN32)
    target_link_libraries(cgpa_calculator PRIVATE ws2_32 wsock32)
endif()
