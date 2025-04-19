#pragma once

#include <string>
#include <unordered_map>
#include <optional>

#include "exlib/core/types.hpp"
#include "exlib/core/exception.hpp"

namespace ex {

class EXLIB_API UserPointer {
public:
	UserPointer() = delete;

	template <class T>
	inline static void set(const std::string& name, T* value);

	template <class T> 
	inline static T* get(const std::string& name);

	inline static void remove(const std::string& name);

	inline static void clear();

	inline static bool find(const std::string& name);

private:
	static std::unordered_map<std::string, void*> pointers;
};

template <class T>
inline void ex::UserPointer::set(const std::string& name, T* value) {
	pointers[name] = (void*) value;
}

template <class T>
inline T* UserPointer::get(const std::string& name) {
	auto iter = pointers.find(name);
	if (iter == pointers.end()) {
		return nullptr;
	}
	return (T*) iter->second;
}

inline void UserPointer::remove(const std::string& name) {
	if (!find(name)) {
		EX_THROW("Unknown user pointer '" + name + "'");
	}
	pointers.erase(name);
}

inline void UserPointer::clear() {
	pointers.clear();
}

inline bool UserPointer::find(const std::string& name) {
	return pointers.find(name) != pointers.end();
}

}
