// HG::Core
#include <GameObject.hpp>
#include <GameObjectCache.hpp>

HG::Core::GameObjectCache& HG::Core::GameObjectCache::i()
{
    static HG::Core::GameObjectCache cache;

    return cache;
}

HG::Core::GameObject* HG::Core::GameObjectCache::create()
{
    if (m_cache.empty())
    {
        return new HG::Core::GameObject();
    }
    else
    {
        auto result = m_cache.back();
        m_cache.pop_back();

        return result;
    }
}

void HG::Core::GameObjectCache::cache(HG::Core::GameObject* gameObject)
{
    m_cache.push_back(gameObject);
}

HG::Core::GameObjectCache::GameObjectCache() :
    m_cache()
{
    m_cache.reserve(128);
}

HG::Core::GameObjectCache::~GameObjectCache()
{
    for (auto&& val : m_cache)
    {
        delete val;
    }

    m_cache.clear();
}
