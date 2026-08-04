cmake_minimum_required(VERSION 3.16)
project(TextEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(text_editor
    src/main.cpp
    src/Editor.cpp
    src/FileManager.cpp
    src/SearchEngine.cpp
    src/MainWindow.cpp
)
set_target_properties(text_editor PROPERTIES
    AUTOMOC_PATH_PREFIX ON
)

target_include_directories(text_editor PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(text_editor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

