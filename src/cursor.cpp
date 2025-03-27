#include "exlib/cursor.hpp"
#include "exlib/window.hpp"

namespace ex {

Vec2d Cursor::get_position() {
    Vec2d pos;
    glfwGetCursorPos(Window::get_instance().get_handle(), &pos.x, &pos.y);
    return pos;
}

void Cursor::set_position(Vec2d pos) {
    glfwSetCursorPos(Window::get_instance().get_handle(), pos.x, pos.y);
}

bool Cursor::is_button_pressed(Button button) {
    return glfwGetMouseButton(Window::get_instance().get_handle(), (int) button) == GLFW_PRESS;
}

void Cursor::hide() {
    glfwSetInputMode(Window::get_instance().get_handle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Cursor::disable() {
    glfwSetInputMode(Window::get_instance().get_handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Cursor::normal() {
    glfwSetInputMode(Window::get_instance().get_handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Cursor::set_position_callback(PositionCallback callback) {
    UserPointer::set("__Ex_Cursor_Callback_Position", callback);

    auto wrapper = +[](GLFWwindow* window, double xpos, double ypos) {
        auto* callback = UserPointer::get<PositionCallback>("__Ex_Cursor_Callback_Position");
        if (callback) {
            (*callback)(Vec2d{ xpos, ypos });
        }
    };

    glfwSetCursorPosCallback(Window::get_instance().get_handle(), wrapper);
}

void Cursor::set_enter_callback(EnterCallback callback) {
    UserPointer::set("__Ex_Cursor_Callback_Enter", callback);

    auto wrapper = +[](GLFWwindow* window, int entered) {
        auto* callback = UserPointer::get<EnterCallback>("__Ex_Cursor_Callback_Enter");
        if (callback) {
            (*callback)(entered == GLFW_TRUE);
        }
    };

    glfwSetCursorEnterCallback(Window::get_instance().get_handle(), wrapper);
}

void Cursor::set_mouse_button_callback(MouseButtonCallback callback) {
    UserPointer::set("__Ex_Cursor_Callback_MouseButton", callback);

    auto wrapper = +[](GLFWwindow* window, int button, int action, int mods) {
        auto* callback = UserPointer::get<MouseButtonCallback>("__Ex_Cursor_Callback_MouseButton");
        if (callback) {
            (*callback)((Button) (button), (Action) (action), mods);
        }
    };

    glfwSetMouseButtonCallback(Window::get_instance().get_handle(), wrapper);
}

void Cursor::set_scroll_callback(ScrollCallback callback) {
    UserPointer::set("__Ex_Cursor_Callback_Scroll", callback);

    auto wrapper = +[](GLFWwindow* window, double xoffset, double yoffset) {
        auto* callback = UserPointer::get<ScrollCallback>("__Ex_Cursor_Callback_Scroll");
        if (callback) {
            (*callback)(Vec2d{ xoffset, yoffset });
        }
    };

    glfwSetScrollCallback(Window::get_instance().get_handle(), wrapper);
}

void Cursor::disable_position_callback() {
    glfwSetCursorPosCallback(Window::get_instance().get_handle(), nullptr);
}

void Cursor::disable_enter_callback() {
    glfwSetCursorEnterCallback(Window::get_instance().get_handle(), nullptr);
}

void Cursor::disable_mouse_button_callback() {
    glfwSetMouseButtonCallback(Window::get_instance().get_handle(), nullptr);
}

void Cursor::disable_scroll_callback() {
    glfwSetScrollCallback(Window::get_instance().get_handle(), nullptr);
}

}