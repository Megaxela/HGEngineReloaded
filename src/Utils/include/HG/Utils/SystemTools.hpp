#pragma once

// C++ STL
#include <cxxabi.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace HG::Utils::SystemTools
{
template <typename T>
std::string getTypeName(const T& t)
{
    int status;
    char* demangled = abi::__cxa_demangle(typeid(t).name(), 0, 0, &status);

    std::string result;

    switch (status)
    {
    case -1:
        throw std::bad_alloc();
    case -2:
        throw std::invalid_argument("Wrong mangled name");
    case -3:
        throw std::invalid_argument("Some argument is invalid");
    default:
        break;
    }

    result = demangled;

    free(demangled);

    return result;
}
} // namespace HG::Utils::SystemTools
