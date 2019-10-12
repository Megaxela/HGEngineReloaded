// HG::Core
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/BuildProperties.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Logging.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Transform.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderBehaviour.hpp>

namespace HG::Core
{
GameObject::GameObject() :
    m_behaviours(),
    m_transform(new (m_cache) Transform(this)),
    m_parentScene(nullptr),
    m_enabled(true),
    m_hidden(false)
{
}

GameObject::~GameObject()
{
    m_parentScene = nullptr;

    m_transform->setParent(nullptr);

    // Merge, to prevent double free
    m_behaviours.merge();

    // Removing behaviours
    for (auto& m_behaviour : m_behaviours)
    {
        delete m_behaviour;
    }

    m_behaviours.clear();

    // Merge, to prevent double free
    m_renderBehaviours.merge();

    for (auto& m_renderBehaviour : m_renderBehaviours)
    {
        delete m_renderBehaviour;
    }

    m_renderBehaviours.clear();

    delete m_transform;
}

void GameObject::update()
{
    // Merging rendering behaviours
    m_renderBehaviours.merge();

    auto newBehaviours = m_behaviours.added();

    // Removing behaviours
    for (auto&& behaviour : m_behaviours.removable())
    {
        delete behaviour;
    }

    // Merging
    m_behaviours.merge();

    // Executing start on new behaviours
    for (auto&& iter : newBehaviours)
    {
        iter->start();
    }

    // Executing update on existing behaviours
    for (auto&& iter : m_behaviours)
    {
        // Will this behaviour removed on
        // next frame. If yes - behaviour
        // will be skipped, because it can
        // be already deleted. So on the next frame
        // this behaviour will be removed from container.
        // If behaviour is disabled, it shouldn't be updated.
        if (m_behaviours.isRemoving(iter) || !iter->isEnabled())
        {
            continue;
        }

        iter->update();
    }
}

void GameObject::removeBehaviour(Behaviour* behaviour)
{
    if constexpr (BuildProperties::isDebug())
    {
        if (behaviour->gameObject() != this)
        {
            HGError() << "Trying to remove behaviour from GameObject, that's not it's parent.";
            return;
        }
    }

    // Remove this gameobject as parent
    behaviour->setParentGameObject(nullptr);
    m_behaviours.remove(behaviour);
}

Scene* GameObject::scene() const
{
    return m_parentScene;
}

void GameObject::setParentScene(Scene* parent)
{
    m_parentScene = parent;
}

void GameObject::addBehaviour(Behaviour* behaviour)
{
    behaviour->setParentGameObject(this);

    switch (behaviour->type())
    {
    case Behaviour::Type::Logic: {
        m_behaviours.add(behaviour);
        break;
    }
    case Behaviour::Type::Render: {
        auto casted = dynamic_cast<HG::Rendering::Base::RenderBehaviour*>(behaviour);

        if (!casted)
        {
            HGError() << "Can't add behaviour of type " << SystemTools::getTypeName(behaviour);
            return;
        }

        m_renderBehaviours.add(casted);
        break;
    }
    }
}

void GameObject::setName(std::string name)
{
    m_name = std::move(name);
}

std::string GameObject::name() const
{
    return m_name;
}

Transform* GameObject::transform()
{
    return m_transform;
}

bool GameObject::isEnabled() const
{
    return m_enabled;
}

void GameObject::setEnabled(bool value)
{
    m_enabled = value;
}

bool GameObject::isHidden() const
{
    return m_hidden;
}

void GameObject::setHidden(bool value)
{
    m_hidden = value;
}
} // namespace HG::Core
