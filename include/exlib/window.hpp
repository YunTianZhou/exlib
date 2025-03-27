#pragma once

#include <memory>
#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include "user_pointer.hpp"
#include "types.hpp"

namespace ex {

    class EXLIB_API Window {
    public:
        friend class Cursor;
        friend class Key;

        inline static Window& create(Vec2i size, std::string _title);
        inline static Window& get_instance();

        Window(Window const&) = delete;
        void operator=(Window const&) = delete;
        ~Window();

        void destroy();

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
        inline void wait_events() const { glfwWaitEvents(); }
        inline void wait_events(double timeout) const { glfwWaitEventsTimeout(timeout); }
        inline void swap_buffers() const { glfwSwapBuffers(window); }

    public:
        /** Callbacks **/

        // Types
        using CloseCallback = void();
        using SizeCallback = void(Vec2i size);
        using FramebufferSizeCallback = void(Vec2i size);
        using PositionCallback = void(Vec2i pos);
        using IconifyCallback = void(bool iconified);
        using MaximizeCallback = void(bool maximized);
        using FocusCallback = void(bool focused);
        using RefreshCallback = void();

        // Setters
        void set_close_callback(CloseCallback callback);
        void set_size_callback(SizeCallback callback);
        void set_framebuffer_size_callback(FramebufferSizeCallback callback);
        void set_position_callback(PositionCallback callback);
        void set_iconify_callback(IconifyCallback callback);
        void set_maximize_callback(MaximizeCallback callback);
        void set_focus_callback(FocusCallback callback);
        void set_refresh_callback(RefreshCallback callback);

        // Disable (Unset) Callbacks
        inline void disable_close_callback() { glfwSetWindowCloseCallback(window, nullptr); }
        inline void disable_size_callback() { glfwSetWindowSizeCallback(window, nullptr); }
        inline void disable_framebuffer_size_callback() { glfwSetFramebufferSizeCallback(window, nullptr); }
        inline void disable_position_callback() { glfwSetWindowPosCallback(window, nullptr); }
        inline void disable_iconify_callback() { glfwSetWindowIconifyCallback(window, nullptr); }
        inline void disable_maximize_callback() { glfwSetWindowMaximizeCallback(window, nullptr); }
        inline void disable_focus_callback() { glfwSetWindowFocusCallback(window, nullptr); }
        inline void disable_refresh_callback() { glfwSetWindowRefreshCallback(window, nullptr); }

    private:
        Window();
        Window(Vec2i size, std::string _title);

        inline GLFWwindow* get_handle() const { return window; }

    private:
        static std::unique_ptr<Window> instance;
        GLFWwindow* window;
        std::string title;
        bool exist;
    };

    inline Window& Window::create(Vec2i size, std::string _title) {
        if (!instance) {
            instance.reset(new Window(size, std::move(_title)));
        }

        return *instance;
    }

    inline Window& Window::get_instance() {
        if (!instance) {
            throw Exception("Window instance has not been initialized.");
        }

        return *instance;
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
        glfwSetWindowTitle(window, _title.c_str());
        title = std::move(_title);
    }

}