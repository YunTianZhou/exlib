#include <iostream>
#include <vector>
#include <string>

#include <exlib/window/window.hpp>
#include <exlib/graphics/rect_shape.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/font.hpp>
#include <exlib/graphics/text.hpp>

int main() {
    // Create a window for testing
    ex::Window& window = ex::Window::create({ 800, 800 }, "Font & Text Test");
    if (!window.is_exist()) {
        std::cerr << "Failed to create window" << std::endl;
        return -1;
    }

    // Load multiple fonts
    ex::Font courier;
    if (!courier.open_from_file(RES_DIR"Courier.ttf")) {
        std::cerr << "Failed to load Courier.ttf" << std::endl;
        return -1;
    }

    ex::Font kai;
    if (!kai.open_from_file(RES_DIR"AR-PL-KaitiM-GB.ttf")) {
        std::cerr << "Failed to load AR-PL-KaitiM-GB.ttf" << std::endl;
        return -1;
    }

    // Prepare a list of Text objects to test various features
    struct TestText {
        ex::Text text;
        ex::Vec2f position;
    };

    std::vector<TestText> tests;

    // 1. Basic string, default style
    ex::Text basic(courier, U"The quick brown fox jumps over the lazy dog", 30);
    basic.set_fill_color(ex::Color::White);
    tests.push_back({ std::move(basic), {20.0f, 20.0f} });

    // 2. Bold + Italic + Underlined
    ex::Text styled(kai, U"粗体+斜体+下划线", 40);
    styled.set_style(ex::Text::Bold | ex::Text::Italic | ex::Text::Underlined);
    styled.set_fill_color(ex::Color::Yellow);
    tests.push_back({ std::move(styled), {20.0f, 80.0f} });

    // 3. StrikeThrough
    ex::Text strike(courier, U"StrikeThrough Example", 35);
    strike.set_style(ex::Text::StrikeThrough);
    strike.set_fill_color(ex::Color::Red);
    tests.push_back({ std::move(strike), {20.0f, 150.0f} });

    // 4. Outline thickness and color
    ex::Text outlined(kai, U"Outline Demo", 50);
    outlined.set_outline_thickness(2.0f);
    outlined.set_outline_color(ex::Color::Blue);
    outlined.set_fill_color(ex::Color::White);
    tests.push_back({ std::move(outlined), {20.0f, 220.0f} });

    // 5. Letter spacing and line spacing
    ex::Text spacing(courier, U"Letter and\nLine Spacing", 30);
    spacing.set_letter_spacing(1.5f);
    spacing.set_line_spacing(1.8f);
    spacing.set_fill_color(ex::Color::Green);
    tests.push_back({ std::move(spacing), {20.0f, 300.0f} });

    // 6. Multiple sizes
    for (int size = 20; size <= 60; size += 10) {
        std::u32string s = U"Size " + std::u32string(1, U'0' + size / 10);
        ex::Text sized(courier, s, size);
        sized.set_fill_color(ex::Color::Magenta);
        tests.push_back({ std::move(sized), {500.0f, 20.0f + (size / 10 - 1) * 50.0f} });
    }

    // Query glyph metrics and kerning for debug
    char32_t first = U'A', second = U'V';
    float kern = courier.get_kerning(first, second, 30);
    std::cout << "Kerning between 'A' and 'V' at size 30: " << kern << std::endl;

    // Underline metrics
    float upos = courier.get_underline_position(30);
    float uthk = courier.get_underline_thickness(30);
    std::cout << "Underline position: " << upos << ", thickness: " << uthk << std::endl;

    // Main loop
    while (window.is_open()) {
        window.clear(ex::Color::Black);

        // Draw each test text
        for (auto& test : tests) {
            test.text.set_position(test.position);
            ex::Draw::draw(test.text);
        }

        window.display();
        window.poll_events();
    }

    window.destroy();
    return 0;
}
