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
        iter->update();
    }
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
    m_behaviours.add(behaviour);
}

void CORE_MODULE_NS::GameObject::addRenderingBehaviour(::RENDERING_BASE_MODULE_NS::RenderBehaviour* renderBehaviour)
{
    m_renderBehaviours.add(renderBehaviour);
}

void CORE_MODULE_NS::GameObject::clear()
{
    m_parentScene = nullptr;

    // Removing behaviours
    for (auto iter = m_behaviours.addedBegin(),
              end  = m_behaviours.addedEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    for (auto iter = m_behaviours.removableBegin(),
              end  = m_behaviours.removableEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    for (auto iter = m_behaviours.begin(),
              end  = m_behaviours.end();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    m_behaviours.clear();

    // Removing rendering behaviours
    for (auto iter = m_renderBehaviours.addedBegin(),
              end  = m_renderBehaviours.addedEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    for (auto iter = m_renderBehaviours.removableBegin(),
              end  = m_renderBehaviours.removableEnd();
         iter != end;
         ++iter)
    {
        delete *iter;
    }

    for (auto iter = m_renderBehaviours.begin(),
              end  = m_renderBehaviours.end();
         iter != end;
         ++iter)
    {
        delete *iter;
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
