#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "exlib/core/config.hpp"

namespace ex {

inline static std::string format(const std::string& func, const std::string& msg) {
    std::ostringstream oss;
    oss << "[EXLIB] [" << func << "] " << msg;
    return oss.str();
}

class EXLIB_API Exception : public std::runtime_error {
public:
    explicit Exception(const std::string& func, const std::string& msg)
        : std::runtime_error(format(func, msg)) {}
};

}

#if defined(__GNUC__) || defined(__clang__)
#define EX_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define EX_FUNC_NAME __FUNCSIG__
#else
#define EX_FUNC_NAME __func__
#endif

#define EX_ERROR(msg)           std::cerr << ex::format(EX_FUNC_NAME, msg) << std::endl
#define EX_THROW(msg)           throw ex::Exception(EX_FUNC_NAME, msg)
