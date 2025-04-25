#include "exlib/window/window.hpp"
#include "exlib/graphics/image.hpp"
#include "exlib/core/user_pointer.hpp"

namespace ex {

std::unique_ptr<Window> Window::instance;

Window::Window() : window(nullptr), exist(false) {}

Window::Window(Vec2i size, std::string _title)
    : window(nullptr), exist(false), title(std::move(_title)) {

    if (!glfwInit()) {
        EX_THROW("GLFW init failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        EX_THROW("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        EX_THROW("GLEW init failed: " + std::string((const char*)(glewGetErrorString(err))));
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
        EX_ERROR("Window is already destroyed");
        return;
    }

    glfwSetWindowShouldClose(window, GL_TRUE);
    glfwDestroyWindow(window);
    exist = false;
}

Window& Window::create(Vec2i size, std::string _title) {
    if (!instance) {
        instance.reset(new Window(size, std::move(_title)));
    }

    return *instance;
}

Window& Window::get_instance() {
    if (!instance) {
        EX_THROW("Window instance has not been initialized");
    }

    return *instance;
}

Vec2i Window::get_size() const {
    Vec2i size;
    glfwGetWindowSize(window, &size.x, &size.y);
    return size;
}

Vec2i Window::get_framebuffer_size() const {
    Vec2i size;
    glfwGetFramebufferSize(window, &size.x, &size.y);
    return size;
}

Vec2i Window::get_position() const {
    Vec2i position;
    glfwGetWindowPos(window, &position.x, &position.y);
    return position;
}

void Window::set_title(std::string _title) {
    glfwSetWindowTitle(window, _title.c_str());
    title = std::move(_title);
}

void Window::set_icon(const Image& image) const {
    set_icon(image.get_size(), image.get_pixels());
}

void Window::set_icon(Vec2i size, const unsigned char* pixels) const {
    GLFWimage images[1];
    images[0].width = size.x;
    images[0].height = size.y;
    images[0].pixels = const_cast<unsigned char*>(pixels);
    glfwSetWindowIcon(window, 1, images);
}

void Window::set_default_icon() const {
    glfwSetWindowIcon(window, 1, nullptr);
}

void ex::Window::clear(Color color) const {
    glClearColor(color.r / 255.0f,
                 color.g / 255.0f,
                 color.b / 255.0f,
                 color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display() const {
    Vec2i framebuffer_size = get_framebuffer_size();
    glViewport(0, 0, framebuffer_size.x, framebuffer_size.y);

    glfwSwapBuffers(window);
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