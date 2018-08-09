#include "GameObject.hpp"
#include "Behaviour.hpp"
#include "Scene.hpp"
#include "RenderBehaviour.hpp"

HG::Core::GameObject::GameObject() :
    m_behaviours(),
    m_transform(this),
    m_parentScene(nullptr),
    m_enabled(true)
{

}

HG::Core::GameObject::~GameObject()
{
    clear();
}

void HG::Core::GameObject::update()
{
    // Merging rendering behaviours
    m_renderBehaviours.merge();

    std::vector<Behaviour*> newBehaviours(m_behaviours.addedBegin(), m_behaviours.addedEnd());

    // Removing behaviours
    for (auto iter = m_behaviours.removableBegin(), end = m_behaviours.removableEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
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
#ifndef NDEBUG
    if (behaviour->gameObject() != this)
    {
        Error() << "Trying to remove behaviour from GameObject, that's not it's parent.";
        return;
    }
#endif

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
    m_behaviours.add(behaviour);
}

void HG::Core::GameObject::addRenderingBehaviour(::HG::Rendering::Base::RenderBehaviour* renderBehaviour)
{
    renderBehaviour->setParentGameObject(this);
    m_renderBehaviours.add(renderBehaviour);
}

void HG::Core::GameObject::removeRenderingBehaviour(::HG::Rendering::Base::RenderBehaviour *renderBehaviour)
{
#ifndef NDEBUG
    if (renderBehaviour->gameObject() != this)
    {
        Error() << "Trying to remove behaviour from GameObject, that's not it's parent.";
        return;
    }
#endif

    // Remove this gameobject as parent
    renderBehaviour->setParentGameObject(nullptr);
    m_renderBehaviours.remove(renderBehaviour);
}

void HG::Core::GameObject::clear()
{
    m_parentScene = nullptr;

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
    return &m_transform;
}

bool HG::Core::GameObject::isEnabled() const
{
    return m_enabled;
}

void HG::Core::GameObject::setEnabled(bool value)
{
    m_enabled = value;
}
