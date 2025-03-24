#include <GL/glew.h>
#include "exlib/window.hpp"

namespace ex {

bool Window::initialized = false;
Window Window::instance;
GLFWwindow* Window::window = nullptr;

Window::Window() : exist(false) {}

Window::Window(Vec2i size, std::string _title)
    : exist(false), title(std::move(_title)) {

    if (!glfwInit()) {
        throw Exception("GLFW init failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw Exception("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw Exception("GLEW init failed");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    initialized = true;
    exist = true;
}

Window::~Window() {
    if (exist) 
        destroy();
    glfwTerminate();
}

void Window::destroy() {
    if (!exist) {
        throw Exception("Window not exist");
    }

    glfwSetWindowShouldClose(window, GL_TRUE);
    glfwDestroyWindow(window);
    exist = false;
}
	
}