#pragma once

#if defined(__linux__) or defined(__linux) or defined(__gnu_linux__) or defined(linux)
#    define OS_LINUX
#elif defined(__EMSCRIPTEN__)
#    define OS_WEB
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#    define OS_WINDOWS
#else
#    error Unknown target system...
#endif

// HG::Utils
#include <HG/Utils/Logging.hpp>

#ifdef OS_LINUX
#    define FUNCTION_WINDOWS_STUB
#    define FUNCTION_WEB_STUB
#    define FUNCTION_LINUX_STUB HGErrorF() << "Function has no implementation for linux.";
#endif

#ifdef OS_WEB
#    define FUNCTION_WINDOWS_STUB
#    define FUNCTION_WEB_STUB HGErrorF() << "Function has no implementation for web.";
#    define FUNCTION_LINUX_STUB
#endif

#ifdef OS_WINDOWS
#    define FUNCTION_WINDOWS_STUB HGErrorF() << "Function has no implementation for windows.";
#    define FUNCTION_WEB_STUB
#    define FUNCTION_LINUX_STUB
#endif
