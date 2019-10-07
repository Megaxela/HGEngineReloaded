// C++ STL
#include <stdexcept>

// HG::Core
#include <HG/Core/CachableObject.hpp>
#include <HG/Core/ResourceCache.hpp>

HG::Core::CachableObject::CachableObject() : m_cache(ResourceCache::currentCache)
{
    ResourceCache::currentCache = nullptr;
}
