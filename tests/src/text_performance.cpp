#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

#include <exlib/window/window.hpp>
#include <exlib/graphics/draw.hpp>
#include <exlib/graphics/font.hpp>
#include <exlib/graphics/text.hpp>

int main() {
    using clock = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;

    // 1) Create window
    ex::Window& window = ex::Window::create({ 1200, 800 }, "Text Performance Test");
    if (!window.is_exist()) {
        std::cerr << "Failed to create window\n";
        return -1;
    }

    // 2) Load fonts
    ex::Font courier, kai;
    if (!courier.open_from_file(RES_DIR"Courier.ttf")) {
        std::cerr << "Failed to load Courier.ttf\n";
        return -1;
    }
    if (!kai.open_from_file(RES_DIR"AR-PL-KaitiM-GB.ttf")) {
        std::cerr << "Failed to load AR-PL-KaitiM-GB.ttf\n";
        return -1;
    }

    // 3) Setup RNG for ASCII vs. CJK
    std::mt19937_64 rng{ std::random_device{}() };
    std::uniform_int_distribution<uint32_t> latin(32u, 126u);
    std::uniform_int_distribution<uint32_t> cjk(0x4E00u, 0x9FFFu);

    // 4) Benchmark parameters
    const size_t N = 500;       // texts per frame
    const size_t L = 64;        // chars per text
    const ms     benchmark_time = ms(5000);  // 5 seconds

    auto start_time = clock::now();
    size_t frame_count = 0;

    // 5) Main loop
    while (window.is_open()) {
        // Timestamp before build+draw
        auto t0 = clock::now();

        window.clear(ex::Color::Black);

        // Build, layout & draw N text objects each frame
        for (size_t i = 0; i < N; ++i) {
            // 10% chance of CJK text
            bool use_cjk = (i % 10 == 0);

            std::u32string s;
            s.reserve(L);
            for (size_t j = 0; j < L; ++j) {
                uint32_t code = use_cjk ? cjk(rng) : latin(rng);
                s.push_back((char32_t) (code));
            }

            // Vary font, size, color
            unsigned int size_pt = 24 + (i % 5) * 4;
            ex::Text txt(use_cjk ? kai : courier, s, size_pt);
            txt.set_fill_color(use_cjk ? ex::Color::Green : ex::Color::White);

            // Position in a grid
            float x = float((i % 25) * 45);
            float y = float(50 + (i / 25) * 30);
            txt.set_position({ x, y });

            ex::Draw::draw(txt);
        }

        // Timestamp after build+draw
        auto t1 = clock::now();

        window.display();
        window.poll_events();

        // Timestamp after swap+events
        auto t2 = clock::now();

        ++frame_count;
        if (t2 - start_time >= benchmark_time) {
            double total_ms = (double) std::chrono::duration_cast<ms>(t2 - start_time).count();
            double avg_fps = frame_count * 1000.0 / total_ms;
            double bd_ms = (double) std::chrono::duration_cast<ms>(t1 - t0).count();
            double sw_ms = (double) std::chrono::duration_cast<ms>(t2 - t1).count();

            std::cout << "--- Performance Results ---\n";
            std::cout << "Frames rendered:    " << frame_count << std::endl;
            std::cout << "Total time (ms):    " << total_ms << std::endl;
            std::cout << "Average FPS:        " << avg_fps << std::endl;
            std::cout << "Last frame Build+Draw (ms): " << bd_ms << std::endl;
            std::cout << "Last frame Swap+Events (ms): " << sw_ms << std::endl;
            break;
        }
    }

    window.destroy();

    std::cin.get();

    return 0;
}
