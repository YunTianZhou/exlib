#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/rect_shape.hpp>

int main() {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    // Create a window
    ex::Window& window = ex::Window::create(ex::Vec2i{ 1200, 600 }, "Rect Shape Performance Test");

    if (!window.is_exist()) {
        std::cerr << "Failed to create window!" << std::endl;
        return -1;
    }

    // Create many rectangles
    const int rect_count = 1000;
    std::vector<ex::RectShape> rects;
    rects.reserve(rect_count);

    for (int i = 0; i < rect_count; ++i) {
        ex::RectShape rect({ 10.0f + (i % 5), 10.0f + (i % 7) });
        rect.set_position({ static_cast<float>((i * 13) % 1200), static_cast<float>((i * 7) % 600) });
        rect.set_fill_color(ex::Color(50 + (i % 200), 100, 150));
        rect.set_outline_color(ex::Color::Blue);
        rect.set_outline_thickness(-1.0f);
        rect.set_origin(rect.get_geometric_center());
        rects.push_back(rect);
    }

    int frame_count = 0;
    TimePoint start_time = Clock::now();

    while (window.is_open()) {
        window.clear();

        // Update and draw all rectangles
        for (auto& rect : rects) {
            rect.rotate(0.5f);
            rect.move({ 0.1f, 0.1f });
            rect.scale({ 1.0005f, 1.0005f });
            ex::Draw::draw(rect);
        }

        window.display();
        window.poll_events();
        ++frame_count;

        // Measure and print performance every 5 seconds
        TimePoint now = Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        if (elapsed.count() >= 5) {
            float fps = frame_count / static_cast<float>(elapsed.count());
            std::cout << "FPS: " << fps << " (" << rect_count << " rectangles)\n";
            frame_count = 0;
            start_time = now;
        }
    }

    window.destroy();
    return 0;
}
