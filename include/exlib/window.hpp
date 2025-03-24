#pragma once

#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include "user_pointer.hpp"
#include "types.hpp"

namespace ex {

    class Window {
    public:
        class Callback {
        public:
            // Types
            using Close = void();
            using Size = void(Vec2i size);
            using FramebufferSize = void(Vec2i size);
            using Position = void(Vec2i pos);
            using Iconify = void(int iconified);
            using Maximize = void(int maximized);
            using Focus = void(int focused);
            using Refresh = void();

            // Setters
            inline void set_close(Close callback) {
                UserPointer::set("__Ex_Window_Callback_Close", callback);

                auto wrapper = +[](GLFWwindow* window) {
                    Close* callback = UserPointer::get<Close>("__Ex_Window_Callback_Close");
                    if (callback) {
                        (*callback)();
                    }
                };

                glfwSetWindowCloseCallback(window, wrapper);
            }

            inline void set_size(Size* callback) {
                UserPointer::set("__Ex_Window_Callback_Size", callback);

                auto wrapper = +[](GLFWwindow* window, int width, int height) {
                    Size* callback = UserPointer::get<Size>("__Ex_Window_Callback_Size");
                    if (callback) {
                        (*callback)(Vec2i{ width, height });
                    }
                };

                glfwSetWindowSizeCallback(window, wrapper);
            }

            inline void set_framebuffer_size(FramebufferSize* callback) {
                UserPointer::set("__Ex_Window_Callback_FramebufferSize", callback);

                auto wrapper = +[](GLFWwindow* window, int width, int height) {
                    FramebufferSize* callback = UserPointer::get<FramebufferSize>("__Ex_Window_Callback_FramebufferSize");
                    if (callback) {
                        (*callback)(Vec2i{ width, height });
                    }
                };

                glfwSetFramebufferSizeCallback(window, wrapper);
            }

            inline void set_position(Position* callback) {
                UserPointer::set("__Ex_Window_Callback_Position", callback);

                auto wrapper = +[](GLFWwindow* window, int xpos, int ypos) {
                    Position* callback = UserPointer::get<Position>("__Ex_Window_Callback_Position");
                    if (callback) {
                        (*callback)(Vec2i{ xpos, ypos });
                    }
                };

                glfwSetWindowPosCallback(window, wrapper);
            }

            inline void set_iconify(Iconify* callback) {
                UserPointer::set("__Ex_Window_Callback_Iconify", callback);

                auto wrapper = +[](GLFWwindow* window, int iconified) {
                    Iconify* callback = UserPointer::get<Iconify>("__Ex_Window_Callback_Iconify");
                    if (callback) {
                        (*callback)(iconified);
                    }
                };

                glfwSetWindowIconifyCallback(window, wrapper);
            }

            inline void set_maximize(Maximize* callback) {
                UserPointer::set("__Ex_Window_Callback_Maximize", callback);

                auto wrapper = +[](GLFWwindow* window, int maximized) {
                    Maximize* callback = UserPointer::get<Maximize>("__Ex_Window_Callback_Maximize");
                    if (callback) {
                        (*callback)(maximized);
                    }
                };

                glfwSetWindowMaximizeCallback(window, wrapper);
            }

            inline void set_focus(Focus* callback) {
                UserPointer::set("__Ex_Window_Callback_Focus", callback);

                auto wrapper = +[](GLFWwindow* window, int focused) {
                    Focus* callback = UserPointer::get<Focus>("__Ex_Window_Callback_Focus");
                    if (callback) {
                        (*callback)(focused);
                    }
                };

                glfwSetWindowFocusCallback(window, wrapper);
            }

            inline void set_refresh(Refresh* callback) {
                UserPointer::set("__Ex_Window_Callback_Refresh", callback);

                auto wrapper = +[](GLFWwindow* window) {
                    Refresh* callback = UserPointer::get<Refresh>("__Ex_Window_Callback_Refresh");
                    if (callback) {
                        (*callback)();
                    }
                };

                glfwSetWindowRefreshCallback(window, wrapper);
            }

            // Disable (Unset) Callbacks
            inline void disable_close() { glfwSetWindowCloseCallback(window, nullptr); }
            inline void disable_size() { glfwSetWindowSizeCallback(window, nullptr); }
            inline void disable_framebuffer_size() { glfwSetFramebufferSizeCallback(window, nullptr); }
            inline void disable_position() { glfwSetWindowPosCallback(window, nullptr); }
            inline void disable_iconify() { glfwSetWindowIconifyCallback(window, nullptr); }
            inline void disable_maximize() { glfwSetWindowMaximizeCallback(window, nullptr); }
            inline void disable_focus() { glfwSetWindowFocusCallback(window, nullptr); }
            inline void disable_refresh() { glfwSetWindowRefreshCallback(window, nullptr); }
        };

    public:
        inline static Window& create(Vec2i size, std::string _title);
        inline static Window& get_instance();

        Window(Window const&) = delete;
        void operator=(Window const&) = delete;
        ~Window();

        void Destroy();

        // Get Functions
        inline int is_exist() const { return exist; }
        inline int is_open() const { return glfwWindowShouldClose(window) == GL_FALSE; }
        inline std::string get_title() const { return title; }

        inline Vec2i get_size() const;
        inline Vec2i get_framebuffer_size() const;
        inline Vec2i get_position() const;
        inline bool is_iconified() const { return glfwGetWindowAttrib(window, GLFW_ICONIFIED); }
        inline bool is_maximized() const { return glfwGetWindowAttrib(window, GLFW_MAXIMIZED); }
        inline bool is_visible() const { return glfwGetWindowAttrib(window, GLFW_VISIBLE); }
        inline bool is_pinned() const { return glfwGetWindowAttrib(window, GLFW_FOCUSED); }
        inline float get_opacity() const { return glfwGetWindowOpacity(window); }

        // Set functions
        inline void set_size(Vec2i size) const { glfwSetWindowSize(window, size.x, size.y); }
        inline void set_position(Vec2i position) const { glfwSetWindowPos(window, position.x, position.y); }
        inline void set_size_limit(Vec2i min, Vec2i max) const { glfwSetWindowSizeLimits(window, min.x, min.y, max.x, max.y); }
        inline void disable_size_limit() const { glfwSetWindowSizeLimits(window, -1, -1, -1, -1); }

        inline void set_title(std::string _title);
        inline void set_icon(int& icon) const = delete;  // Not implemented
        inline void set_icon(int icons[], int size) const = delete;  // Not implemented
        inline void set_default_icon() const { glfwSetWindowIcon(window, 0, nullptr); }

        inline void set_aspect_ratio(int numer, int denom) const { glfwSetWindowAspectRatio(window, numer, denom); }
        inline void disable_aspect_ratio() const { glfwSetWindowAspectRatio(window, -1, -1); }
        inline void set_opacity(float opacity) const { glfwSetWindowOpacity(window, opacity); }
        inline void set_swap_interval(int interval) const { glfwSwapInterval(interval); }

        inline void iconify() const { glfwIconifyWindow(window); }
        inline void maximize() const { glfwMaximizeWindow(window); }
        inline void restore() const { glfwRestoreWindow(window); }
        inline void enable_resize() const { glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_TRUE); }
        inline void disable_resize() const { glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_FALSE); }

        inline void hide() const { glfwHideWindow(window); }
        inline void show() const { glfwShowWindow(window); }
        inline void focus() const { glfwFocusWindow(window); }
        inline void request_attention() const { glfwRequestWindowAttention(window); }

        // Other functions
        inline void poll_events() const { glfwPollEvents(); }
        inline void swap_buffers() const { glfwSwapBuffers(window); }

    private:
        Window();
        Window(Vec2i size, std::string _title);

    private:
        static bool initialized;
        static Window instance;
        static GLFWwindow* window;
        std::string title;
        bool exist;
    };

    inline Window& Window::create(Vec2i size, std::string _title) {
        static Window instance(size, std::move(_title));
        return instance;
    }

    inline Window& Window::get_instance() {
        if (!initialized) {
            throw Exception("Window instance has not been initialized!");
        }

        return instance;
    }

    inline Vec2i Window::get_size() const {
        Vec2i size;
        glfwGetWindowSize(window, &size.x, &size.y);
        return size;
    }

    inline Vec2i Window::get_framebuffer_size() const {
        Vec2i size;
        glfwGetFramebufferSize(window, &size.x, &size.y);
        return size;
    }

    inline Vec2i Window::get_position() const {
        Vec2i position;
        glfwGetWindowPos(window, &position.x, &position.y);
        return position;
    }

    inline void Window::set_title(std::string _title) {
        glfwSetWindowTitle(window, title.c_str());
        title = std::move(_title);
    }

}