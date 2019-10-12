// HG::Core
#include <HG/Core/CachableObject.hpp>
#include <HG/Core/ResourceCache.hpp>

namespace HG::Core
{
CachableObject::CachableObject() : m_cache(ResourceCache::currentCache)
{
    ResourceCache::currentCache = nullptr;
}
}
