// HG::Core
#include <HG/Core/ResourceCache.hpp>
#include <HG/Core/CachableObject.hpp>

// HG::Utils
#include <CurrentLogger.hpp>

thread_local HG::Core::ResourceCache* HG::Core::ResourceCache::currentCache = nullptr;

HG::Core::ResourceCache::ResourceCache() :
        m_objects()
{

}

HG::Core::ResourceCache::~ResourceCache()
{
    for (auto& cache : m_objects)
    {
        if (!cache.second.used.empty())
        {
            Error() << "Leaked cached resources detected. " << cache.first << " hash - " << cache.second.used.size() << " leaked objects.";
        }

        for (auto& element : cache.second.used)
        {
            element->~CachableObject();
            ::delete[] ((uint8_t*) element);
        }

        for (auto& element : cache.second.available)
        {
            ::delete[] ((uint8_t*) element);
        }
    }
}
