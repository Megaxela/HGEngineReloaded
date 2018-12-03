#pragma once

// C++ STL
#include <unordered_set>
#include <unordered_map>
#include <typeinfo>
#include <cstring>

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
        template<typename T>
        void* getResource(std::size_t n)
        {
            // Searching for cache by type
            auto cache = m_objects.find(typeid(T).hash_code());

            if (cache == m_objects.end())
            {
                cache = m_objects.insert(std::make_pair(typeid(T).hash_code(), TypeCache())).first;
            }

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

            cache->second.available.erase(obj);
            cache->second.used.insert(*obj);

            return *obj;
        }

        /**
         * @brief Method for caching resource that was
         * create in current cache instance.
         * @tparam T Actual resource type.
         * @param obj Pointer to cachable object.
         */
        template<typename T>
        void cacheResource(CachableObject* obj)
        {
            auto cache = m_objects.find(typeid(T).hash_code());

            if (cache == m_objects.end())
            {
                throw std::runtime_error("Trying to cache resource, that was created in another cache.");
            }

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
        template<typename T>
        std::unordered_set<T*> getUsedResources() const
        {
            std::unordered_set<T*> result;

            auto cache = m_objects.find(typeid(T).hash_code());

            if (cache == m_objects.end())
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
            std::unordered_set<CachableObject *> used;
            std::unordered_set<CachableObject *> available;
        };

        std::unordered_map<
                std::size_t,
                TypeCache
        > m_objects;
    };
}

