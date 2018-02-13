#include "Behaviour.hpp"

CORE_MODULE_NS::Behaviour::Behaviour()
{

}

void CORE_MODULE_NS::Behaviour::update()
{
    onUpdate();
}

void CORE_MODULE_NS::Behaviour::start()
{
    onStart();
}

void CORE_MODULE_NS::Behaviour::render()
{
    onRender();
}

void CORE_MODULE_NS::Behaviour::onStart()
{

}

void CORE_MODULE_NS::Behaviour::onUpdate()
{

}

void CORE_MODULE_NS::Behaviour::onFixedUpdate()
{

}

void CORE_MODULE_NS::Behaviour::onRender()
{

}

CORE_MODULE_NS::GameObject* CORE_MODULE_NS::Behaviour::gameObject() const
{
    return m_parent;
}

void CORE_MODULE_NS::Behaviour::setParentGameObject(CORE_MODULE_NS::GameObject* ptr)
{
    m_parent = ptr;
}
