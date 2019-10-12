// C++ STL
#include <utility>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/GameObjectBuilder.hpp>
#include <HG/Core/Transform.hpp>

// ALogger
#include <CurrentLogger.hpp>

namespace HG::Core
{
GameObjectBuilder::operator GameObject*()
{
    return deploy();
}

GameObject* GameObjectBuilder::deploy()
{
    auto result = m_currentGameObject;

    m_currentGameObject = nullptr;

    return result;
}

GameObjectBuilder& GameObjectBuilder::setHidden(bool hidden)
{
    m_currentGameObject->setHidden(hidden);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setEnabled(bool enabled)
{
    m_currentGameObject->setEnabled(enabled);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::addBehaviour(Behaviour* behaviour)
{
    m_currentGameObject->addBehaviour(behaviour);

    return (*this);
}

GameObjectBuilder::GameObjectBuilder(ResourceCache* cache) :
    m_currentGameObject(new (cache) GameObject)
{
}

GameObjectBuilder& GameObjectBuilder::setGameObject(GameObject* ptr)
{
    delete m_currentGameObject;

    m_currentGameObject = ptr;

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setLocalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setLocalPosition(position);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setGlobalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setGlobalPosition(position);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setRotation(const glm::quat& rotation)
{
    m_currentGameObject->transform()->setLocalRotation(rotation);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setScale(const glm::vec3& scale)
{
    m_currentGameObject->transform()->setLocalScale(scale);

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setName(std::string name)
{
    m_currentGameObject->setName(std::move(name));

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setParent(GameObject* parent)
{
    if (parent == nullptr)
    {
        m_currentGameObject->transform()->setParent(nullptr);
    }
    else
    {
        m_currentGameObject->transform()->setParent(parent->transform());
    }

    return (*this);
}

GameObjectBuilder& GameObjectBuilder::setParent(Transform* parent)
{
    m_currentGameObject->transform()->setParent(parent);

    return (*this);
}
}