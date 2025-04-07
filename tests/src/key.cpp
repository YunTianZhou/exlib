#include <iostream>

#include <exlib/window/window.hpp>
#include <exlib/window/key.hpp>

void on_button(ex::Key::Type key, int scancode, ex::Key::Action action, int mods) {
    if (key == ex::Key::Type::A) {
        std::cout << "A pressed!" << std::endl;
    }
}

void on_char(unsigned int codepoint) {
    if (codepoint < 128) {
        std::cout << "Character: " << (char) (codepoint) << std::endl;
    }
    else {
        std::cout << "Unicode character: U+" << std::hex << codepoint << std::dec << std::endl;
    }
}

int main() {
    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 800, 600 }, "Cursor Test");

    if (!window.is_exist())
        return -1;

    // Set callbacks
    ex::Key::set_button_callback(on_button);
    ex::Key::set_char_callback(on_char);

    // Test basic cursor functions
    std::cout << "Key name of CapsLock: " << ex::Key::get_key_name(ex::Key::Type::CapsLock) << std::endl;

    // Main event loop
    while (window.is_open()) {
        window.clear();
        window.poll_events();
        window.display();
    }

    // Destroy the window
    window.destroy();
}
