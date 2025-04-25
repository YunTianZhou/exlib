#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "exlib/core/types.hpp"

namespace ex {

class Image;

class EXLIB_API Window {
public:
    friend class Cursor;
    friend class Key;

    static Window& create(Vec2i size, std::string _title);
    static Window& get_instance();

    Window(Window const&) = delete;
    void operator=(Window const&) = delete;
    ~Window();

    void destroy();

    // Get Functions
    inline int is_exist() const { return exist; }
    inline int is_open() const { return glfwWindowShouldClose(window) == GL_FALSE; }
    inline std::string get_title() const { return title; }

    Vec2i get_size() const;
    Vec2i get_framebuffer_size() const;
    Vec2i get_position() const;
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

    void set_title(std::string _title);
    void set_icon(const Image& image) const;
    void set_icon(Vec2i size, const unsigned char* pixels) const;
    void set_default_icon() const;

    inline void set_aspect_ratio(int numer, int denom) const { glfwSetWindowAspectRatio(window, numer, denom); }
    inline void disable_aspect_ratio() const { glfwSetWindowAspectRatio(window, -1, -1); }
    inline void set_opacity(float opacity) const { glfwSetWindowOpacity(window, opacity); }
    inline void set_display_interval(int interval) const { glfwSwapInterval(interval); }

    inline void iconify() const { glfwIconifyWindow(window); }
    inline void maximize() const { glfwMaximizeWindow(window); }
    inline void restore() const { glfwRestoreWindow(window); }
    inline void enable_resize() const { glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_TRUE); }
    inline void disable_resize() const { glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_FALSE); }

    inline void hide() const { glfwHideWindow(window); }
    inline void show() const { glfwShowWindow(window); }
    inline void focus() const { glfwFocusWindow(window); }
    inline void request_attention() const { glfwRequestWindowAttention(window); }

    // Events
    inline void poll_events() const { glfwPollEvents(); }
    inline void wait_events() const { glfwWaitEvents(); }
    inline void wait_events(double timeout) const { glfwWaitEventsTimeout(timeout); }

    // Clear / Display
    void clear(Color color = Color::Black) const;
    void display() const;

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

    GLFWwindow* get_handle() const { return window; }

private:
    static std::unique_ptr<Window> instance;
    GLFWwindow* window;
    std::string title;
    bool exist;
};

}