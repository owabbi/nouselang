#ifndef ERROR_H
#define ERROR_H

#include <stdexcept>
#include <string>
#include "utils.h"

inline void throwError(const std::string& message, int line = -1) {
    if (line >= 0)
        throw std::runtime_error("Line " + std::to_string(line) + ": " + red(message));
    else
        throw std::runtime_error("Error: " + red(message));
}

#endif // ERROR_H
