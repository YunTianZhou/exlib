
# exlib

**exlib** is a lightweight, modern C++ graphics library C++ graphics library built on top of OpenGL. It provides utilities for window management, event handling, graphics, and OpenGL interfaces.

## Features

- Easy creation and control of the window

- Input event managing (like key presses and mouse movements)

- Provides basic graphic interfaces

- Support texture rendering

- Cross-platform compatibility (Windows & Linux)

## Examples

For more examples, go to `exlib/tests`, which contains examples for all functions in this project.

## Getting Started

### Prerequisites

- A C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.5 or higher

### Clone the Library

```bash
git clone https://github.com/YunTianZhou/exlib.git
cd exlib
```

## Build

Run the following commands based on your system:
- If you want to build the library, go to directory `exlib/`.
- If you want to build the examples, go to directory `exlib/tests/`.

### Windows

```bash
cd scripts
build.bat
```

### Linux

```bash
cd scripts
chmod +x build.sh
./build.sh
```

The compiled binaries will be generate in `./build/`.

## Usage

Here's a simple template of how to setup an `exlib` project:

```cpp
#include <exlib/exlib.hpp>

int main() {
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Window Test");

    if (!window.is_exist())
        return -1;

    while (window.is_open()) {
        window.clear();

        // Draw something here
        // ex::Draw(...)

        window.display();
        window.poll_events();
    }

    window.destroy();
}
```

## Acknowledgments

This project utilizes the following open-source libraries:

- [GLFW](https://github.com/glfw/glfw) - Multi-platform library for OpenGL
- [GLEW](https://github.com/nigels-com/glew) - The OpenGL Extension Wrangler Library
- [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics
- [STB](https://github.com/nothings/stb) - Image import and export

This project references to the design of `Graphics` in [SFML](https://github.com/SFML/SFML).
