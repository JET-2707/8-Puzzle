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
│   ├── CustomBoardsDialog.qml
│   ├── ScoresMenu.qml
│   └── Board.qml
└── README.md
```

# Clone

```bash
git clone https://github.com/JET-2707/8-Puzzle.git

cd 8-Puzzle
```

# Compile

## Requirements

To compile the project is needed:

- Qt 6, and the modules:
 - Qt Quick
 - Qt QML
 - Qt Quick Controls
- CMake 
- Make
- A C++ compatible compiler(GCC, Clang or MSVC).

### Linux:

Install Qt 6 and development tools using your distribution's package manager or the official Qt installer.

On Fedora, for example:

```bash
sudo dnf install cmake make gcc-c++ qt6-qtbase-devel qt6-qtdeclarative-devel
```

# Build

From the project root directory, run:

```bash
cmake -S . -B build
cmake --build build
```

# Run

### Linux:

Once compiled, run the application using:

```bash
./build/puzzleApp
```

# Technologies Used

- C++ programming language.
- QML.
- Qt.
- CMake.
- Make.
- Git and GitHub.
