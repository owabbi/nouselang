#ifndef UTILS_H
#define UTILS_H

#include <iostream>

inline std::string red(const std::string& msg) {
    return "\033[31m" + msg + "\033[0m";
}

inline std::string green(const std::string& msg) {
    return "\033[32m" + msg + "\033[0m";
}

#endif