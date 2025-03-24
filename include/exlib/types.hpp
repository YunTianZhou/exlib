#pragma once

#include <exception>
#include <string>

#include "config.hpp"

namespace ex {

template<typename T>
struct EXLIB_API Vec2 { T x, y; };

using Vec2i = Vec2<int>;

class EXLIB_API Exception : public std::exception {
private:
    std::string message;

public:
    explicit Exception(const std::string& msg) : message("[EXLIB] " + msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

}