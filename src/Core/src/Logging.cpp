// HG::Core
#include <HG/Core/Logging.hpp>

// ALogger
#include <Loggers/BasicLogger.hpp>

LoggerPtr HG::Core::Logging::userLogger()
{
    static auto logger = std::make_shared<Loggers::BasicLogger>();

    return logger;
}
