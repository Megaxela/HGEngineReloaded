#include "ResourceManager.hpp"

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

