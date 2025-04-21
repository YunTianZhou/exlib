
# exlib

**exlib** is a lightweight, modern C++ graphics library built on top of OpenGL. It provides utilities for window management, event handling, graphics, and OpenGL interfaces.

## Features

- Easy creation and control of windows

- Input event management (e.g., key presses and mouse movements)

- Basic graphics interfaces

- Texture rendering support

- Cross-platform compatibility (Windows & Linux)

## Examples

For more examples, see the `exlib/tests` directory, which contains usage examples for all functions in this project.

## Getting Started

### Prerequisites

- A C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- CMake 3.5 or higher

### Clone the Library

```bash
git clone --recurse-submodules https://github.com/YunTianZhou/exlib.git
cd exlib
```

## Build

Run the following commands based on your system:
- To build the library, go to the `exlib` directory.
- To build the examples, go to the `exlib/tests` directory.

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

The compiled binaries will be generated in `./build`.

## Usage

Here's a simple template for setting up an `exlib` project:

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

This project is inspired by the design of the `Graphics` module in [SFML](https://github.com/SFML/SFML).
