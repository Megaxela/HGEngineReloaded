#pragma once

#include "GameObject.hpp"

namespace HG::Core
{
    /**
     * @brief Singleton class, that describes
     * cache for gameobject.
     */
    class GameObjectCache
    {
    public:

        // Disable copying
        GameObjectCache(const GameObjectCache& rhs) = delete;
        GameObjectCache& operator=(const GameObjectCache&) = delete;

        /**
         * @brief Method for getting singleton instance.
         * @return Reference to cache.
         */
        static GameObjectCache& i();

        /**
         * @brief Method for creating gameobject.
         * It can be taken from cache or new one
         * can be allocated.
         * @return Pointer to game object.
         */
        GameObject* create();

        /**
         * @brief Method for caching gameobject.
         * @param gameObject Pointer to game object.
         */
        void cache(GameObject* gameObject);

    private:

        /**
         * @brief Private constructor (Singleton)
         */
        GameObjectCache();

        /**
         * @brief Destructor.
         */
        ~GameObjectCache();

        std::vector<GameObject*> m_cache;
    };
}

