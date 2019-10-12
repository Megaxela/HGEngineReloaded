#pragma once

// C++ STL
#include <cstring>
#include <shared_mutex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

// HG::Core
#include <HG/Core/CachableObject.hpp>

namespace HG::Core
{
/**
 * @brief Cache, that performs resource caching.
 * And provides access to objects, that's
 * in use now.
 */
class ResourceCache
{
public:
    // Monkey coding.
    // This variable only required to set
    // resource cache to HG::Core::CachableObject in it's constructor.
    // Because changing initialization order is wrong.
    static thread_local ResourceCache* currentCache;

    /**
     * @brief Constructor.
     */
    ResourceCache();

    /**
     * @brief Destructor.
     */
    ~ResourceCache();

    /**
     * @brief Method, that performs resource allocation
     * if required.
     * @tparam T Resource type.
     * @param n Number of elements.
     * @todo Figure out what to do with multiple objects.
     * @return Pointer to memory for objects.
     */
    template <typename T> [[nodiscard]]
    void* getResource(std::size_t n)
    {
        // Searching for cache by type
        decltype(m_objects)::iterator cache;

        {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            cache = m_objects.find(typeid(T).hash_code());
        }

        if (std::shared_lock<std::shared_mutex>(m_mutex), cache == m_objects.end())
        {
            std::unique_lock<std::shared_mutex> lock(m_mutex);
            cache = m_objects.insert(std::make_pair(typeid(T).hash_code(), TypeCache())).first;
        }

        std::unique_lock<std::shared_mutex> lock(cache->second.mutex);
        // If there is no available element - create new one
        if (cache->second.available.empty())
        {
            void* raw = new uint8_t[n * sizeof(T)];

            cache->second.used.insert(static_cast<CachableObject*>(raw));

            // After this call - constructor will be called,
            // and ResourceCache::currentCache will be nullptr again.
            ResourceCache::currentCache = this;

            return raw;
        }

        // Otherwise - return first cached.
        auto obj = cache->second.available.begin();

        auto resource = *obj;
        cache->second.available.erase(obj);
        cache->second.used.insert(resource);

        // After this call - constructor will be called,
        // and ResourceCache::currentCache will be nullptr again.
        ResourceCache::currentCache = this;

        return resource;
    }

    /**
     * @brief Method for caching resource that was
     * create in current cache instance.
     * @tparam T Actual resource type.
     * @param obj Pointer to cachable object.
     */
    template <typename T>
    void cacheResource(CachableObject* obj)
    {
        decltype(m_objects)::iterator cache;

        {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            cache = m_objects.find(typeid(T).hash_code());
        }

        if (std::shared_lock<std::shared_mutex>(m_mutex), cache == m_objects.end())
        {
            throw std::runtime_error("Trying to cache resource, that was created in another cache.");
        }

        std::unique_lock<std::shared_mutex> lock(cache->second.mutex);
        auto elem = cache->second.used.find(obj);

        if (elem == cache->second.used.end())
        {
            throw std::runtime_error("Trying to cache resource, that was created in another cache.");
        }

        cache->second.used.erase(elem);
        cache->second.available.insert(obj);
    }

    /**
     * @brief Method for getting resource of specific type
     * that's currently in use.
     * @tparam T Type of resources to find.
     * @return Unordered set of elements.
     */
    template <typename T> [[nodiscard]]
    std::unordered_set<T*> getUsedResources() const
    {
        std::unordered_set<T*> result;

        decltype(m_objects)::const_iterator cache;

        {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            cache = m_objects.find(typeid(T).hash_code());
        }

        if (std::shared_lock<std::shared_mutex>(m_mutex), cache == m_objects.end())
        {
            return result;
        }

        for (auto& elem : cache->second.used)
        {
            result.insert(dynamic_cast<T*>(elem));
        }

        return result;
    }

private:
    struct TypeCache
    {
        TypeCache() = default;

        TypeCache(const TypeCache& rhs) : used(rhs.used), available(rhs.available), mutex()
        {
        }

        std::unordered_set<CachableObject*> used;
        std::unordered_set<CachableObject*> available;
        mutable std::shared_mutex mutex;
    };

    mutable std::shared_mutex m_mutex;
    std::unordered_map<std::size_t, TypeCache> m_objects;
};
} // namespace HG::Core
