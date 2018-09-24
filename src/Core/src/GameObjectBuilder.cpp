// C++ STL
#include <utility>

// HG::Core
#include <GameObjectBuilder.hpp>
#include <GameObjectCache.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

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

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::addBehaviour(HG::Core::Behaviour* behaviour)
{
    m_currentGameObject->addBehaviour(behaviour);

    return (*this);
}

HG::Core::GameObjectBuilder::GameObjectBuilder() :
    m_currentGameObject(nullptr)
{
    pickCurrentGameObject();
}

void HG::Core::GameObjectBuilder::pickCurrentGameObject()
{
    if (m_currentGameObject != nullptr)
    {
        Debug() << "Previous builder call skipped deploy part. Clearing it.";
        m_currentGameObject->clear();
    }
    else
    {
        m_currentGameObject = HG::Core::GameObjectCache::i().create();
    }
}

HG::Core::GameObjectBuilder& HG::Core::GameObjectBuilder::setGameObject(HG::Core::GameObject* ptr)
{
    if (m_currentGameObject != nullptr)
    {
        HG::Core::GameObjectCache::i().cache(m_currentGameObject);
    }

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
