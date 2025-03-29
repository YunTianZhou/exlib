#include "exlib/window/key.hpp"
#include "exlib/window/window.hpp"

namespace ex {

	std::string Key::get_key_name(Type key, int scancode) {
		const char* name = glfwGetKeyName((int) (key), scancode);
		return name ? std::string(name) : "Unknown";
	}

	int Key::get_key_scancode(Type key) {
		return glfwGetKeyScancode((int) (key));
	}

	bool Key::is_key_pressed(Type key) {
		return glfwGetKey(Window::get_instance().get_handle(), (int) (key)) == GLFW_PRESS;
	}

	void Key::set_button_callback(ButtonCallback callback) {
		UserPointer::set("__Ex_Key_Callback_Button", callback);

		auto wrapper = +[](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* callback = UserPointer::get<ButtonCallback>("__Ex_Key_Callback_Button");
			if (callback) {
				(*callback)((Type) key, scancode, (Action) action, mods);
			}
		};

		glfwSetKeyCallback(Window::get_instance().get_handle(), wrapper);
	}

	void Key::set_char_callback(CharCallback callback) {
		UserPointer::set("__Ex_Key_Callback_Char", callback);

		auto wrapper = +[](GLFWwindow* window, unsigned int codepoint) {
			auto* callback = UserPointer::get<CharCallback>("__Ex_Key_Callback_Char");
			if (callback) {
				(*callback)(codepoint);
			}
		};

		glfwSetCharCallback(Window::get_instance().get_handle(), wrapper);
	}

	void Key::disable_button_callback() {
		glfwSetKeyCallback(Window::get_instance().get_handle(), nullptr);
	}

	void Key::disable_char_callback() {
		glfwSetCharCallback(Window::get_instance().get_handle(), nullptr);
	}

}
