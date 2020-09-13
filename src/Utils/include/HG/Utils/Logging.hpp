#pragma once

// C++ STL
#include <cstring>

// spdlog
#include <spdlog/spdlog.h>

#define HGDebug(...) SPDLOG_DEBUG(__VA_ARGS__)

#define HGInfo(...) SPDLOG_INFO(__VA_ARGS__)

#define HGWarning(...) SPDLOG_WARN(__VA_ARGS__)

#define HGError(...) SPDLOG_ERROR(__VA_ARGS__)

namespace HG::Utils
{
/**
 * @brief Class, that describes user logging system.
 * This system uses ALogger library as backend.
 */
class Logging
{
public:
    /**
     * @brief Method for getting user logger.
     * @return Pointer to user logger.
     */
    //    static LoggerPtr userLogger();
    //
    //    static void overrideUserLogger(LoggerPtr logger);

private:
    //    static LoggerPtr m_override;
};
} // namespace HG::Utils
