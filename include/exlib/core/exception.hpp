#pragma once

#include <string>
#include <exception>

#include "config.hpp"

namespace ex {

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