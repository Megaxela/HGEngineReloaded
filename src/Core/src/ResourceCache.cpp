// HG::Core
#include <HG/Core/CachableObject.hpp>
#include <HG/Core/ResourceCache.hpp>
#include <HG/Core/Logging.hpp>

namespace HG::Core
{
thread_local ResourceCache* ResourceCache::currentCache = nullptr;

ResourceCache::ResourceCache() : m_objects()
{
}

ResourceCache::~ResourceCache()
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
            HGError() << "Leaked cached resources detected. " << cache.first << " hash - " << cache.second.used.size()
                    << " leaked objects.";
        }
    }
}
}