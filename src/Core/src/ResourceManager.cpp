// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/ResourceAccessor.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Core
{
ResourceManager::ResourceManager(Application* parent) : m_accessor(nullptr), m_application(parent)
{
}

ResourceManager::~ResourceManager()
{
    delete m_accessor;
}

Application* ResourceManager::application() const
{
    return m_application;
}

ResourceAccessor* ResourceManager::resourceAccessor() const
{
    return m_accessor;
}

DataPtr ResourceManager::loadRawFromAccessor(const std::string& id)
{
    HGInfo("Loading resource \"{}\"", id);

    if (m_accessor == nullptr)
    {
        HGError("Trying to load \"{}\" resource, without ResourceAccessor.", id);
        return nullptr;
    }

    auto data = m_accessor->loadRaw(id);

    if (data == nullptr)
    {
        HGError("Can't load \"{}\" resource. See errors above.", id);

        return nullptr;
    }

    return data;
}

void ResourceManager::setResourceAccessor(ResourceAccessor* accessor)
{
    if (m_accessor != nullptr)
    {
        HGWarning("Redefining ResourceAccessor accessor is bad practice.");
    }

    m_accessor = accessor;
}
} // namespace HG::Core
