#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "exlib/core/types.hpp"

namespace ex {

class EXLIB_API Cursor {
public:
    enum class Button : int {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
        Last = GLFW_MOUSE_BUTTON_LAST,

        Button4 = GLFW_MOUSE_BUTTON_4,
        Button5 = GLFW_MOUSE_BUTTON_5,
        Button6 = GLFW_MOUSE_BUTTON_6,
        Button7 = GLFW_MOUSE_BUTTON_7,
    };

    enum class Action : int {
        Release = GLFW_RELEASE,
        Press = GLFW_PRESS,
    };

    enum class Mod : int {
        Shift = GLFW_MOD_SHIFT,
        Control = GLFW_MOD_CONTROL,
        Alt = GLFW_MOD_ALT,
        Super = GLFW_MOD_SUPER,
        CapsLock = GLFW_MOD_CAPS_LOCK,
        NumLock = GLFW_MOD_NUM_LOCK,
    };

public:
    Cursor() = delete;

    static Vec2d get_position();
    static void set_position(Vec2d pos);
    static bool is_button_pressed(Button button);
    static void hide();
    static void disable();
    static void normal();

    inline static bool is_mod_active(int mods, Mod mod) { return mods & (int) mod; }

public:
    /** Callbacks **/

    // Types
    using PositionCallback = void(Vec2d pos);
    using EnterCallback = void(bool enter);
    using MouseButtonCallback = void(Button button, Action action, int mods);
    using ScrollCallback = void(Vec2d offset);
    
    // Setters
    static void set_position_callback(PositionCallback callback);
    static void set_enter_callback(EnterCallback callback);
    static void set_mouse_button_callback(MouseButtonCallback callback);
    static void set_scroll_callback(ScrollCallback callback);

    // Disable (Unset) Callbacks
    static void disable_position_callback();
    static void disable_enter_callback();
    static void disable_mouse_button_callback();
    static void disable_scroll_callback();

};

}
