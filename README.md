# 8 Puzzle


# Project Structure

```
8-Puzzle/
├── headers/
│   ├── algorithms.hpp
│   ├── board.hpp
│   ├── controller.hpp
│   ├── core.hpp
│   ├── files.hpp
│   └── modes.hpp
├── src/
│   ├── algorithms.cpp
│   ├── board.cpp
│   ├── controller.cpp
│   ├── core.cpp
│   ├── files.cpp
│   ├── modes.cpp
│   └── 8-puzzle.cpp
├── gui/
│   ├── Main.qml
│   ├── TopBar.qml
│   ├── StepControls.qml
│   └── Board.qml
└── README.md
```

## Clone, Build and Run
Clone the repository:
```bash
git clone https://github.com/JET-2707/8-Puzzle.git

cd 8-Puzzle
```

Compile the project:


### Linux:
Create the file "CMakeLists.txt".

```txt
cmake_minimum_required(VERSION 3.16)

project(puzzleApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

if(COMMAND qt_policy)
    qt_policy(SET QTP0001 NEW)
endif()

qt_add_executable(puzzleApp
    src/8-puzzle.cpp
)

set_source_files_properties(gui/Main.qml
    PROPERTIES QT_RESOURCE_ALIAS Main.qml
)

set_source_files_properties(gui/Board.qml
    PROPERTIES QT_RESOURCE_ALIAS Board.qml
)

set_source_files_properties(gui/TopBar.qml
    PROPERTIES QT_RESOURCE_ALIAS TopBar.qml
)

set_source_files_properties(gui/StepControls.qml
    PROPERTIES QT_RESOURCE_ALIAS StepControls.qml
)

qt_add_qml_module(puzzleApp
    URI Puzzle
    VERSION 1.0

    QML_FILES
        gui/Main.qml
        gui/Board.qml
        gui/TopBar.qml
        gui/StepControls.qml

    SOURCES
        headers/algorithms.hpp
        headers/controller.hpp
        headers/files.hpp
        headers/board.hpp
        headers/core.hpp
        headers/modes.hpp
        src/algorithms.cpp
        src/controller.cpp
        src/files.cpp
        src/board.cpp
        src/core.cpp
        src/modes.cpp
)

target_link_libraries(puzzleApp
    PRIVATE
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

target_include_directories(puzzleApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)
```

```bash
cmake -S . -B build
cmake --build build
```

Run the executable:
```bash
./build/puzzleApp
```

## Technologies Used
- C++ programming language.
- QML.
- Qt.
- Git and GitHub.
