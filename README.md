
# exlib

**exlib** is a lightweight, modern C++ graphics library built on top of OpenGL. It provides utilities for window management, event handling, graphics, and OpenGL interfaces.

## Features

-   Easy creation and control of windows
    
-   Input event management (e.g., key presses and mouse movements)
    
-   Basic graphics interfaces
    
-   Texture rendering support
    
-   Cross-platform compatibility (Windows & Linux)
    

## Examples

For more examples, see the `exlib/tests` directory, which contains usage examples for all functions in this project.

## Getting Started

### Prerequisites

-   A C++17 compatible compiler (e.g., GCC, Clang, MSVC)
    
-   CMake 3.5 or higher
    

### Clone the Library

```bash
git clone --recurse-submodules https://github.com/YunTianZhou/exlib.git
cd exlib
```

## Build

You can build the project using either standard CMake commands or a convenient wrapper script.

### Option 1: Manual CMake Invocation

#### 1. Configure the build

```bash
cmake -B build -S . \
  -DBUILD_TESTS=<ON|OFF> \
  -DBUILD_STATIC=<ON|OFF> \
  -DBUILD_SHARED=<ON|OFF> \
  -DTESTS_LINK_SHARED=<ON|OFF>
```

-   `BUILD_TESTS` (default: OFF) — whether to build the test suite
    
-   `BUILD_STATIC` (default: ON) — whether to build the static library
    
-   `BUILD_SHARED` (default: ON) — whether to build the shared library

-   `DTESTS_LINK_SHARED` (default: OFF) — whether to link tests against the shared library (static library as default)
    

You can change the options or CMake configs according to your needs.

#### 2. Build the project

```bash
cmake --build build --config <Release|Debug>
```

### Option 2: Use the Wrapper Script

A script is provided to simplify the build process. Just run:

```bash
scripts/build.sh [options]
```

It will generate a build directory at the project root (`exlib/build`), including the generated binaries.

#### Script options

```
  -c, --config              Release|Debug       (default: Release)
  -t, --tests               ON|OFF              (default: ON)
  -t, --link-shared-tests   ON|OFF              (default: OFF)
  -g, --generator           <name>              e.g. "Ninja", "Unix Makefiles"
  -a, --arch                <arch>              e.g. x64 (for multi-config generators)
  -h, --help                                    Show this help message
```

#### Example usage

This sets up a Debug build, disables tests, and uses Ninja to build:

```bash
scripts/build.sh --config Debug --tests OFF --generator "Ninja"
```

To use the default settings: Release build, tests ON, and the default generator (e.g., Unix Makefiles or Ninja if available):

```bash
scripts/build.sh
```

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
        // ex::Draw::draw(...)

        window.display();
        window.poll_events();
    }

    window.destroy();
}
```

## Acknowledgments

This project utilizes the following open-source libraries:

-   [GLFW](https://github.com/glfw/glfw) - Multi-platform library for OpenGL
    
-   [GLEW](https://github.com/nigels-com/glew) - The OpenGL Extension Wrangler Library
    
-   [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics
    
-   [STB](https://github.com/nothings/stb) - Image import and export
    

This project is inspired by the design of the `Graphics` module in [SFML](https://github.com/SFML/SFML).
