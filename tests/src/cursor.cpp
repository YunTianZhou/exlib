#include <iostream>

#include <exlib/window/window.hpp>
#include <exlib/window/cursor.hpp>

void on_position(ex::Vec2d pos) {
    std::cout << "Cursor position: (" << pos.x << ", " << pos.y << ")" << std::endl;
}

void on_mouse_button(ex::Cursor::Button button, ex::Cursor::Action action, int mods) {
    if (button == ex::Cursor::Button::Left && action == ex::Cursor::Action::Press) {
        std::cout << "Left mouse button pressed!" << std::endl;
    }
}

void on_enter(bool enter) {
    if (enter)
        std::cout << "Cursor entered the window!" << std::endl;
    else
        std::cout << "Cursor left the window!" << std::endl;
}

void on_mouse_scroll(ex::Vec2d offset) {
    std::cout << "Scroll offset: (" << offset.x << ", " << offset.y << ")" << std::endl;
}

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Cursor Test");

    if (!window.is_exist())
        return -1;

    // Set callbacks
    ex::Cursor::set_position_callback(on_position);
    ex::Cursor::set_mouse_button_callback(on_mouse_button);
    ex::Cursor::set_enter_callback(on_enter);
    ex::Cursor::set_scroll_callback(on_mouse_scroll);

    // Test basic cursor functions
    ex::Cursor::set_position(ex::Vec2d{ 100, 100 });
    ex::Cursor::hide();
    ex::Cursor::normal();

    // Main event loop
    while (window.is_open()) {
        window.clear();
        window.poll_events();
        window.display();
    }

    // Destroy the window
    window.destroy();
}
