// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Utils
{
    Logging::Span::Span(std::string name)
    {
        HG::Utils::Logging::instance().pushNamespace(std::move(name));
    }

    Logging::Span::~Span()
    {
        HG::Utils::Logging::instance().popNamespace();
    }

    Logging::Logging() :
        m_namespaces()
    {

    }

    Logging& Logging::instance()
    {
        thread_local static Logging logging;
        return logging;
    }

    void Logging::pushNamespace(std::string ns)
    {
        m_namespaces.emplace_back(std::move(ns));
        m_baked_namespace = fmt::format("{}", fmt::join(m_namespaces, "::"));
    }

    void Logging::popNamespace()
    {
        m_namespaces.pop_back();
        m_baked_namespace = fmt::format("{}", fmt::join(m_namespaces, "::"));
    }

    const std::string& Logging::bakedNamespace()
    {
        return m_baked_namespace;
    }
} // namespace HG::Utils
