#include "Behaviour.hpp"

CORE_MODULE_NS::Behaviour::Behaviour() :
    m_enabled(true),
    m_parent(nullptr),
    m_properties()
{

}

CORE_MODULE_NS::Behaviour::~Behaviour()
{
    if (m_parent)
    {
        m_parent->removeBehaviour(this);
        m_parent = nullptr;
    }
}

bool CORE_MODULE_NS::Behaviour::isEnabled() const
{
    return m_enabled;
}

void CORE_MODULE_NS::Behaviour::setEnabled(bool value)
{
    m_enabled = value;
}

const CORE_MODULE_NS::Input *CORE_MODULE_NS::Behaviour::input() const
{
    return scene()->application()->input();
}

void CORE_MODULE_NS::Behaviour::update()
{
    onUpdate();
}

void CORE_MODULE_NS::Behaviour::start()
{
    onStart();
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

CORE_MODULE_NS::GameObject* CORE_MODULE_NS::Behaviour::gameObject() const
{
    return m_parent;
}

void CORE_MODULE_NS::Behaviour::setParentGameObject(CORE_MODULE_NS::GameObject* ptr)
{
    m_parent = ptr;
}

CORE_MODULE_NS::Scene* CORE_MODULE_NS::Behaviour::scene() const
{
    if (m_parent == nullptr)
    {
        return nullptr;
    }

    return m_parent->scene();
}

void CORE_MODULE_NS::Behaviour::addProperty(CORE_MODULE_NS::Behaviour::Property property)
{
    // todo: Add checking if property with this name already exists
    m_properties.emplace_back(property.name(), property);
}
