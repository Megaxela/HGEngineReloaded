#include <Platform.hpp>
#include "PhysicalResource.hpp"

#ifdef OS_LINUX
#include <sys/sysinfo.h>
#endif

uint64_t UTILS_MODULE_NS::PhysicalResource::getTotalRAM()
{
    FUNCTION_WINDOWS_STUB;
#ifdef OS_LINUX
    struct sysinfo info;

    if (sysinfo(&info) == -1)
    {
        ErrorF() << "sysinfo error: " << strerror(errno);
        return 0;
    }

    return info.totalram;
#endif
}

uint64_t UTILS_MODULE_NS::PhysicalResource::getFreeRAM()
{
    FUNCTION_WINDOWS_STUB;
#ifdef OS_LINUX
    struct sysinfo info;

    if (sysinfo(&info) == -1)
    {
        ErrorF() << "sysinfo error: " << strerror(errno);
        return 0;
    }

    return info.freeram;
#endif
}

namespace
{
    uint64_t parseLine(char *line)
    {
        // This assumes that a digit will be found and the line ends in " Kb".
        auto i = strlen(line);
        const char *p = line;

        // Skipping all symbols, except digits
        while (*p < '0' || *p > '9') p++;

        line[i - 3] = '\0';

        // Parsing
        return static_cast<uint64_t>(atoll(p));
    }
}

uint64_t UTILS_MODULE_NS::PhysicalResource::getProcessRAMUsed()
{
    FUNCTION_WINDOWS_STUB;
#ifdef OS_LINUX
    // todo: Replace with ifstream
    FILE* file = fopen("/proc/self/status", "r");

    if (file == nullptr)
    {
        ErrorF() << "Can't open \"/proc/self/status\". Error: " << strerror(errno);
        return 0;
    }

    char line[128];

    while (fgets(line, 128, file) != nullptr)
    {
        // VmRSS (Memory + Shared Mem)
        // RssAnon (only application resources)
        if (strncmp(line, "RssAnon:", 6) == 0)
        {
            fclose(file);
            return parseLine(line) * 1000; // kB
        }
    }

    WarningF() << "VmRSS was not found in \"/proc/self/status\".";
    fclose(file);
    return 0;

#endif
}
