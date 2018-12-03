// HG::Core
#include <HG/Core/ResourceCache.hpp>
#include <HG/Core/CachableObject.hpp>

thread_local HG::Core::ResourceCache* HG::Core::ResourceCache::currentCache = nullptr;

HG::Core::ResourceCache::ResourceCache() :
        m_objects()
{

}

HG::Core::ResourceCache::~ResourceCache()
{
    for (auto& cache : m_objects)
    {
        for (auto& element : cache.second.available)
        {
            ::delete element;
        }

        for (auto& element : cache.second.used)
        {
            ::delete element;
        }
    }
}
