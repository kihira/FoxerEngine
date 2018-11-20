#include <iostream>
#include "Logger.h"

namespace Logger {
    void error(const char *message) {
        std::cerr << message << std::endl;
    }
}