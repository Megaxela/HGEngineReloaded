// HG::Utils
#include <HG/Utils/Logging.hpp>

// ALogger
#include <Loggers/BasicLogger.hpp>

namespace HG::Utils
{
LoggerPtr Logging::m_override = nullptr;

LoggerPtr Logging::userLogger()
{
    static auto logger = std::make_shared<Loggers::BasicLogger>();

    if (m_override)
    {
        return m_override;
    }

    return logger;
}

void Logging::overrideUserLogger(LoggerPtr logger)
{
    m_override = std::move(logger);
}
} // namespace HG::Utils
