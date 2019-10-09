// HG::Utils
#include <HG/Utils/PhysicalResource.hpp>
#include <HG/Utils/Platform.hpp>

// Linux
#ifdef OS_LINUX
#    include <sys/sysinfo.h>
#endif

#ifdef OS_WINDOWS
#    include <windows.h>
// hotfix
#    include <psapi.h>
#endif

uint64_t HG::Utils::PhysicalResource::getTotalRAM()
{
#ifdef OS_WINDOWS
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&status))
    {
        return 0;
    }

    return status.ullTotalPhys;
#endif

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

uint64_t HG::Utils::PhysicalResource::getFreeRAM()
{
#ifdef OS_WINDOWS
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(MEMORYSTATUSEX);

    if (!GlobalMemoryStatusEx(&status))
    {
        return 0;
    }

    return status.ullAvailPhys;
#endif

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
uint64_t parseLine(char* line)
{
    // This assumes that a digit will be found and the line ends in " Kb".
    auto i        = strlen(line);
    const char* p = line;

    // Skipping all symbols, except digits
    while (*p < '0' || *p > '9')
        p++;

    line[i - 3] = '\0';

    // Parsing
    return static_cast<uint64_t>(atoll(p));
}
} // namespace

uint64_t HG::Utils::PhysicalResource::getProcessRAMUsed()
{
#ifdef OS_WINDOWS

    return 0;
//    auto currentProcess = GetCurrentProcess();
//
//    if (currentProcess == nullptr)
//    {
//        return 0;
//    }
//
//    PROCESS_MEMORY_COUNTERS counters;
//
//    if (!GetProcessMemoryInfo(currentProcess, &counters, sizeof(counters)))
//    {
//        return 0;
//    }
//
//    return counters.PagefileUsage;
#endif
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
