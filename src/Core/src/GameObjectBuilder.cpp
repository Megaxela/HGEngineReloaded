// C++ STL
#include <utility>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/GameObjectBuilder.hpp>
#include <HG/Core/Transform.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Core::GameObjectBuilder::operator GameObject*()
{
    return deploy();
}

HG::Core::GameObject* HG::Core::GameObjectBuilder::deploy()
{
    auto result = m_currentGameObject;

    m_currentGameObject = nullptr;

    return result;
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setHidden(bool hidden)
{
    m_currentGameObject->setHidden(hidden);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setEnabled(bool enabled)
{
    m_currentGameObject->setEnabled(enabled);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::addBehaviour(HG::Core::Behaviour* behaviour)
{
    m_currentGameObject->addBehaviour(behaviour);

    return (*this);
}

HG::Core::GameObjectBuilder::GameObjectBuilder(HG::Core::ResourceCache* cache) :
    m_currentGameObject(new (cache) HG::Core::GameObject)
{
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setGameObject(HG::Core::GameObject* ptr)
{
    delete m_currentGameObject;

    m_currentGameObject = ptr;

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setLocalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setLocalPosition(position);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setGlobalPosition(const glm::vec3& position)
{
    m_currentGameObject->transform()->setGlobalPosition(position);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setRotation(const glm::quat& rotation)
{
    m_currentGameObject->transform()->setLocalRotation(rotation);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setScale(const glm::vec3& scale)
{
    m_currentGameObject->transform()->setLocalScale(scale);

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setName(std::string name)
{
    m_currentGameObject->setName(std::move(name));

    return (*this);
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setParent(HG::Core::GameObject* parent)
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

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setParent(HG::Core::Transform* parent)
{
    m_currentGameObject->transform()->setParent(parent);

    return (*this);
}
