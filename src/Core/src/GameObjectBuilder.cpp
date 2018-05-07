#include <CurrentLogger.hpp>
#include <GameObjectCache.hpp>
#include <utility>
#include "GameObjectBuilder.hpp"

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
    pickCurrentGameObject();
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

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setGameObject(CORE_MODULE_NS::GameObject* ptr)
{
    if (m_currentGameObject != nullptr)
    {
        GameObjectCache::i().cache(m_currentGameObject);
    }

    m_currentGameObject = ptr;

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setLocalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setLocalPosition(position);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setGlobalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setGlobalPosition(position);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setRotation(const glm::quat& rotation)
{
    m_currentGameObject->transform()->setLocalRotation(rotation);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setScale(const glm::vec3& scale)
{
    m_currentGameObject->transform()->setLocalScale(scale);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setName(std::string name)
{
    m_currentGameObject->setName(std::move(name));

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setParent(CORE_MODULE_NS::GameObject* parent)
{
    m_currentGameObject->transform()->setParent(parent->transform());

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::setParent(CORE_MODULE_NS::Transform* parent)
{
    m_currentGameObject->transform()->setParent(parent);

    return (*this);
}

CORE_MODULE_NS::GameObjectBuilder& CORE_MODULE_NS::GameObjectBuilder::addRenderingBehaviour(RENDERING_BASE_MODULE_NS::RenderBehaviour* renderingBehaviour)
{
    m_currentGameObject->addRenderingBehaviour(renderingBehaviour);

    return (*this);
}
