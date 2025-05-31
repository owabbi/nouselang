#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>
#include <string>
#include "utils.h"

inline void throwError(const std::string& message) {
    throw std::runtime_error("[NoUse Error] " + red(message));
}

#endif // ERROR_H
