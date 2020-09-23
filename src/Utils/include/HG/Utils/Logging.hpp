#pragma once

// C++ STL
#include <cstring>
#include <stack>

// spdlog
#include <spdlog/spdlog.h>

#define HGDebug(FMT, ...) SPDLOG_DEBUG(fmt::format("[{}] {}", ::HG::Utils::Logging::instance().bakedNamespace(), FMT) __VA_OPT__(,) __VA_ARGS__ )

#define HGInfo(FMT, ...) SPDLOG_INFO(fmt::format("[{}] {}", ::HG::Utils::Logging::instance().bakedNamespace(), FMT) __VA_OPT__(,) __VA_ARGS__ )

#define HGWarning(FMT, ...) SPDLOG_WARN(fmt::format("[{}] {}", ::HG::Utils::Logging::instance().bakedNamespace(), FMT) __VA_OPT__(,) __VA_ARGS__ )

#define HGError(FMT, ...) SPDLOG_ERROR(fmt::format("[{}] {}", ::HG::Utils::Logging::instance().bakedNamespace(), FMT) __VA_OPT__(,) __VA_ARGS__ )

namespace HG::Utils
{
/**
 * @brief Class, that describes extra data
 * for logging system. Namespaces for example.
 */
class Logging
{
public:
    class Span
    {
    public:
        explicit Span(std::string name);

        ~Span();
    };

    Logging();

    static Logging& instance();

    void pushNamespace(std::string ns);

    void popNamespace();

    const std::string& bakedNamespace();

private:
    std::vector<std::string> m_namespaces;
    std::string m_baked_namespace;
};
} // namespace HG::Utils
