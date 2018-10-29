// HG::Core
#include <HG/Core/ResourceManager.hpp>
#include <HG/Core/ResourceAccessor.hpp>
#include <HG/Core/Application.hpp>

HG::Core::ResourceManager::ResourceManager(HG::Core::Application* parent) :
    m_accessor(nullptr),
    m_application(parent)
{

}

HG::Core::ResourceManager::~ResourceManager()
{
    delete m_accessor;
}

HG::Core::Application* HG::Core::ResourceManager::application()
{
    return m_application;
}

HG::Core::ResourceAccessor* HG::Core::ResourceManager::resourceAccessor() const
{
    return m_accessor;
}

HG::Core::DataPtr HG::Core::ResourceManager::loadRawFromAccessor(const std::string &id)
{
    Info() << "Loading resource \"" << id << "\"";

    if (m_accessor == nullptr)
    {
        Error() << "Trying to load \"" << id << "\" resource, without ResourceAccessor.";
        return nullptr;
    }

    auto data = m_accessor->loadRaw(id);

    if (data == nullptr)
    {
        Error() << "Can't load \"" << id << "\" resource. See errors above.";

        return nullptr;
    }

    return data;
}

void HG::Core::ResourceManager::setResourceAccessor(HG::Core::ResourceAccessor* accessor)
{
    if (m_accessor != nullptr)
    {
        Warning() << "Redefining ResourceAccessor accessor is bad practice.";
    }

    m_accessor = accessor;
}
