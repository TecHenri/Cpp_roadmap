### Lancer le projet

**Prérequis** : Qt 6.x (MinGW 64-bit), CMake ≥ 3.16

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
.\text_editor.exe
```

---

## 🇬🇧 English

### Description

A full-featured desktop text editor built in C++ with Qt 6.
Supports creating, editing, opening and saving text files
through a native graphical interface.

### Concepts covered

| Category | Details |
|----------|---------|
| **Modern C++17** | Classes, namespaces, Qt inheritance |
| **Qt 6** | QMainWindow, QTextEdit, signals/slots, AUTOMOC |
| **File I/O** | QFile, QTextStream, UTF-8 encoding |
| **Regex** | QRegularExpression, find/replace |
| **Architecture** | UI / business logic separation |

### Features

- New, Open, Save, Save As
- Undo / Redo, Cut / Copy / Paste, Select All
- Case-insensitive find and replace
- Status bar: line, column, word count
- Unsaved changes detection

### How to run

**Requirements** : Qt 6.x (MinGW 64-bit), CMake ≥ 3.16

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
.\text_editor.exe
```

### Why MainWindow.hpp is in src/ instead of include/

Qt's AUTOMOC system requires that headers containing `Q_OBJECT`
be in the same directory as their corresponding `.cpp` files.
`Editor.hpp`, `FileManager.hpp` and `SearchEngine.hpp` have no
Qt macros so they stay in `include/`.

---

## License

MIT