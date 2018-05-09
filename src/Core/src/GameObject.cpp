#include "GameObject.hpp"
#include "Behaviour.hpp"
#include "Scene.hpp"
#include "RenderBehaviour.hpp"

CORE_MODULE_NS::GameObject::GameObject() :
    m_behaviours(),
    m_transform(this),
    m_parentScene(nullptr)
{

}

CORE_MODULE_NS::GameObject::~GameObject()
{
    clear();
}

void CORE_MODULE_NS::GameObject::update()
{
    // Merging rendering behaviours
    m_renderBehaviours.merge();

    // Executing start on new behaviours
    for (auto iter = m_behaviours.addedBegin(), end = m_behaviours.addedEnd();
         iter != end;
         ++iter)
    {
        (*iter)->start();
    }

    // Removing behaviours
    for (auto iter = m_behaviours.removableBegin(), end = m_behaviours.removableEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    // Merging
    m_behaviours.merge();

    // Executing update on existing behaviours
    for (auto&& iter : m_behaviours)
    {
        // Will this behaviour removed on 
        // next frame. If yes - behaviour
        // will be skipped, because it can
        // be already deleted. So on the next frame
        // this behaviour will be removed from container.
        if (m_behaviours.isRemoving(iter))
        {
            continue;
        }
        
        iter->update();
    }
}

void CORE_MODULE_NS::GameObject::removeBehaviour(CORE_MODULE_NS::Behaviour *behaviour)
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

CORE_MODULE_NS::Scene* CORE_MODULE_NS::GameObject::scene() const
{
    return m_parentScene;
}

void CORE_MODULE_NS::GameObject::setParentScene(CORE_MODULE_NS::Scene* parent)
{
    m_parentScene = parent;
}

void CORE_MODULE_NS::GameObject::addBehaviour(CORE_MODULE_NS::Behaviour* behaviour)
{
    behaviour->setParentGameObject(this);
    m_behaviours.add(behaviour);
}

void CORE_MODULE_NS::GameObject::addRenderingBehaviour(::RENDERING_BASE_MODULE_NS::RenderBehaviour* renderBehaviour)
{
    renderBehaviour->setParentGameObject(this);
    m_renderBehaviours.add(renderBehaviour);
}

void CORE_MODULE_NS::GameObject::removeRenderingBehaviour(::RENDERING_BASE_MODULE_NS::RenderBehaviour *renderBehaviour)
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

void CORE_MODULE_NS::GameObject::clear()
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

void CORE_MODULE_NS::GameObject::setName(std::string name)
{
    m_name = std::move(name);
}

std::string CORE_MODULE_NS::GameObject::name() const
{
    return m_name;
}

CORE_MODULE_NS::Transform* CORE_MODULE_NS::GameObject::transform()
{
    return &m_transform;
}
