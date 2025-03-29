#include <GL/glew.h>
#include "exlib/window/window.hpp"

namespace ex {

std::unique_ptr<Window> Window::instance;

Window::Window() : window(nullptr), exist(false) {}

Window::Window(Vec2i size, std::string _title)
    : window(nullptr), exist(false), title(std::move(_title)) {

    if (!glfwInit()) {
        throw Exception("GLFW init failed.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw Exception("GLFW window creation failed.");
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw Exception("GLEW init failed.");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    exist = true;
}

Window::~Window() {
    if (exist) 
        destroy();
    glfwTerminate();
}

void Window::destroy() {
    if (!exist) {
        throw Exception("Window not exist.");
    }

    glfwSetWindowShouldClose(window, GL_TRUE);
    glfwDestroyWindow(window);
    exist = false;
}

void Window::set_close_callback(CloseCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Close", callback);

    auto wrapper = +[](GLFWwindow* window) {
        CloseCallback* callback = UserPointer::get<CloseCallback>("__Ex_Window_Callback_Close");
        if (callback) {
            (*callback)();
        }
    };

    glfwSetWindowCloseCallback(window, wrapper);
}

void Window::set_size_callback(SizeCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Size", callback);

    auto wrapper = +[](GLFWwindow* window, int width, int height) {
        SizeCallback* callback = UserPointer::get<SizeCallback>("__Ex_Window_Callback_Size");
        if (callback) {
            (*callback)(Vec2i{ width, height });
        }
    };

    glfwSetWindowSizeCallback(window, wrapper);
}

void Window::set_framebuffer_size_callback(FramebufferSizeCallback callback) {
    UserPointer::set("__Ex_Window_Callback_FramebufferSize", callback);

    auto wrapper = +[](GLFWwindow* window, int width, int height) {
        FramebufferSizeCallback* callback = UserPointer::get<FramebufferSizeCallback>("__Ex_Window_Callback_FramebufferSize");
        if (callback) {
            (*callback)(Vec2i{ width, height });
        }
    };

    glfwSetFramebufferSizeCallback(window, wrapper);
}

void Window::set_position_callback(PositionCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Position", callback);

    auto wrapper = +[](GLFWwindow* window, int xpos, int ypos) {
        PositionCallback* callback = UserPointer::get<PositionCallback>("__Ex_Window_Callback_Position");
        if (callback) {
            (*callback)(Vec2i{ xpos, ypos });
        }
    };

    glfwSetWindowPosCallback(window, wrapper);
}

void Window::set_iconify_callback(IconifyCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Iconify", callback);

    auto wrapper = +[](GLFWwindow* window, int iconified) {
        IconifyCallback* callback = UserPointer::get<IconifyCallback>("__Ex_Window_Callback_Iconify");
        if (callback) {
            (*callback)(iconified == GLFW_TRUE);
        }
    };

    glfwSetWindowIconifyCallback(window, wrapper);
}

void Window::set_maximize_callback(MaximizeCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Maximize", callback);

    auto wrapper = +[](GLFWwindow* window, int maximized) {
        MaximizeCallback* callback = UserPointer::get<MaximizeCallback>("__Ex_Window_Callback_Maximize");
        if (callback) {
            (*callback)(maximized == GLFW_TRUE);
        }
    };

    glfwSetWindowMaximizeCallback(window, wrapper);
}

void Window::set_focus_callback(FocusCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Focus", callback);

    auto wrapper = +[](GLFWwindow* window, int focused) {
        FocusCallback* callback = UserPointer::get<FocusCallback>("__Ex_Window_Callback_Focus");
        if (callback) {
            (*callback)(focused == GLFW_TRUE);
        }
    };

    glfwSetWindowFocusCallback(window, wrapper);
}

void Window::set_refresh_callback(RefreshCallback callback) {
    UserPointer::set("__Ex_Window_Callback_Refresh", callback);

    auto wrapper = +[](GLFWwindow* window) {
        RefreshCallback* callback = UserPointer::get<RefreshCallback>("__Ex_Window_Callback_Refresh");
        if (callback) {
            (*callback)();
        }
    };

    glfwSetWindowRefreshCallback(window, wrapper);
}
	
}