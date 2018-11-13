// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/BuildProperties.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderBehaviour.hpp>

// ALogger library
#include <CurrentLogger.hpp>

HG::Core::GameObject::GameObject() :
    m_behaviours(),
    m_transform(new HG::Core::Transform(this)),
    m_parentScene(nullptr),
    m_enabled(true),
    m_hidden(false)
{

}

HG::Core::GameObject::~GameObject()
{
    clear();
    delete m_transform;
}

void HG::Core::GameObject::update()
{
    // Merging rendering behaviours
    m_renderBehaviours.merge();

    const auto& newBehaviours = m_behaviours.added();

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
        if (m_behaviours.isRemoving(iter) ||
            !iter->isEnabled())
        {
            continue;
        }
        
        iter->update();
    }
}

void HG::Core::GameObject::removeBehaviour(HG::Core::Behaviour *behaviour)
{
    if constexpr (HG::Core::BuildProperties::isDebug())
    {
        if (behaviour->gameObject() != this)
        {
            Error() << "Trying to remove behaviour from GameObject, that's not it's parent.";
            return;
        }
    }

    // Remove this gameobject as parent
    behaviour->setParentGameObject(nullptr);
    m_behaviours.remove(behaviour);
}

HG::Core::Scene* HG::Core::GameObject::scene() const
{
    return m_parentScene;
}

void HG::Core::GameObject::setParentScene(HG::Core::Scene* parent)
{
    m_parentScene = parent;
}

void HG::Core::GameObject::addBehaviour(HG::Core::Behaviour* behaviour)
{
    behaviour->setParentGameObject(this);

    switch (behaviour->type())
    {
    case Behaviour::Type::Logic:
    {
        m_behaviours.add(behaviour);
        break;
    }
    case Behaviour::Type::Render:
    {
        auto casted = dynamic_cast<HG::Rendering::Base::RenderBehaviour*>(behaviour);

        if (!casted)
        {
            Error() << "Can't add behaviour of type " << SystemTools::getTypeName(behaviour);
            return;
        }

        m_renderBehaviours.add(casted);
        break;
    }
    }
}

void HG::Core::GameObject::clear()
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
}

void HG::Core::GameObject::setName(std::string name)
{
    m_name = std::move(name);
}

std::string HG::Core::GameObject::name() const
{
    return m_name;
}

HG::Core::Transform* HG::Core::GameObject::transform()
{
    return m_transform;
}

bool HG::Core::GameObject::isEnabled() const
{
    return m_enabled;
}

void HG::Core::GameObject::setEnabled(bool value)
{
    m_enabled = value;
}

bool HG::Core::GameObject::isHidden() const
{
    return m_hidden;
}

void HG::Core::GameObject::setHidden(bool value)
{
    m_hidden = value;
}
