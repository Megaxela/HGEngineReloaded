#pragma once

#if defined(__linux__) or defined(__linux) or defined(__gnu_linux__) or defined(linux)
#    define OS_LINUX
#else
#    define OS_WINDOWS
#endif

// HG::Utils
#include <HG/Utils/Logging.hpp>

#ifdef OS_LINUX
#    define FUNCTION_WINDOWS_STUB
#    define FUNCTION_LINUX_STUB HGErrorF() << "Function has no realisation for linux.";
#else
#    define FUNCTION_WINDOWS_STUB HGErrorF() << "Function has no realisation for windows.";
#    define FUNCTION_LINUX_STUB
#endif
