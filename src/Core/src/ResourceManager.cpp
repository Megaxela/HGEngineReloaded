#include <ResourceManager.hpp>
#include <ResourceAccessor.hpp>

HG::Core::ResourceManager::ResourceManager() :
    m_accessor(nullptr)
{

}

HG::Core::ResourceManager::~ResourceManager()
{
    delete m_accessor;
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

