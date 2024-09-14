# File Explorer

A Qt-based file explorer application with customizable settings and a file system model, built using C++20 and Qt (supports both Qt5 and Qt6).

## Features

- Responsive file explorer using Qt Widgets
- Custom file system model to interact with the file system
- Separate window for managing default settings, currently under development.

## Prerequisites

### Dependencies
Before building the project, ensure you have the following installed:

1. **CMake** (version 3.5 or higher)
2. **Qt** (Qt5 or Qt6):
   - Qt Widgets module
3. **C++ Compiler**:
   - Supports C++20 (e.g., GCC 11+, Clang 10+)

### Installing Qt and CMake

#### Ubuntu (with Qt5):
```bash
sudo apt install cmake qtbase5-dev qtchooser qt5-qmake
```

## Building the Project
1. Clone the repository
```
git clone https://github.com/Walec75/file-explorer.git/
cd file-explorer
```
2. Create a build directory
```
cmake -S . -Bbuild
```
3. Build and run the project
```
cd build
make && ./file_explorer
```
