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
    // It would be nice to perform
    // clean here, but it's impossible, cause
    // we doesn't have dependency tree.
    // So if one cachable resource contains
    // another one will get double free.
    for (auto& cache : m_objects)
    {
        if (!cache.second.used.empty())
        {
            Error() << "Leaked cached resources detected. " << cache.first << " hash - " << cache.second.used.size() << " leaked objects.";
        }
    }
}
