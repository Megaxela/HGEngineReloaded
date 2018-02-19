#include "ResourceManager.hpp"

CORE_MODULE_NS::ResourceManager::ResourceManager() :
    m_accessor(nullptr)
{

}

CORE_MODULE_NS::ResourceManager::~ResourceManager()
{
    delete m_accessor;
}

CORE_MODULE_NS::ResourceAccessor* CORE_MODULE_NS::ResourceManager::resourceAccessor() const
{
    return m_accessor;
}

