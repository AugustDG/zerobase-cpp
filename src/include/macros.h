#pragma once

#include <iostream>

#include "result.hpp"

#define INFO(msg) std::cout << (msg) << std::endl
#define WARN(msg) std::cout << (msg) << std::endl
#define ERROR(msg) std::cerr << (msg) << std::endl

#define IF_INVALID_RESULT_RETURN(result) \
    if (!result)                         \
    return result