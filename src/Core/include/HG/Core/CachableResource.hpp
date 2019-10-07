#pragma once

// HG::Core
#include <HG/Core/CachableObject.hpp>
#include <HG/Core/ResourceCache.hpp>

namespace HG::Core
{
/**
 * @brief Class, that describes resource, that can be cached.
 * @tparam RealResourceType Real resource type.
 */
template <typename RealResourceType>
class CachableResource : public CachableObject
{
public:
    /**
     * @brief Overriding over placement new for resource allocation / getting from cache.
     * @param n Amount of elements.
     * @param cache Pointer to resource cache.
     * @return Pointer to raw data.
     */
    void* operator new(std::size_t n, ResourceCache* cache)
    {
        return cache->getResource<RealResourceType>(n);
    }

    /**
     * @brief Overriding over delete for resource caching.
     * @param ptr Pointer to memory that contains object for caching.
     */
    void operator delete(void* ptr)
    {
        auto* obj = static_cast<CachableResource<RealResourceType>*>(ptr);

        obj->m_cache->template cacheResource<RealResourceType>(obj);
    }
};
} // namespace HG::Core
