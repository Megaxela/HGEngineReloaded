#include <CurrentLogger.hpp>
#include <GameObjectCache.hpp>
#include "GameObjectBuilder.hpp"

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::i()
{
    static GameObjectBuilder builder;

    builder.pickCurrentGameObject();

    return builder;
}

CORE_MODULE_NS::GameObjectBuilder::operator GameObject*()
{
    auto result = m_currentGameObject; 
    
    m_currentGameObject = nullptr;
    
    return result;
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::addBehaviour(CORE_MODULE_NS::Behaviour* behaviour)
{
    m_currentGameObject->addBehaviour(behaviour);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder::GameObjectBuilder() :
    m_currentGameObject(nullptr)
{

}

void CORE_MODULE_NS::GameObjectBuilder::pickCurrentGameObject()
{
    if (m_currentGameObject != nullptr)
    {
        Debug() << "Previous builder call skipped deploy part. Clearing it.";
        m_currentGameObject->clear();
    }
    else
    {
        m_currentGameObject = GameObjectCache::i().create();
    }
}
