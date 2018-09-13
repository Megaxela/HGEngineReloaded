
// HG::Core
#include <Behaviour.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <Application.hpp>

HG::Core::Behaviour::Behaviour(Type t) :
    m_type(t),
    m_enabled(true),
    m_parent(nullptr),
    m_properties()
{

}

HG::Core::Behaviour::~Behaviour()
{
    if (m_parent)
    {
        m_parent->removeBehaviour(this);
        m_parent = nullptr;
    }
}

HG::Core::Behaviour::Type HG::Core::Behaviour::type() const
{
    return m_type;
}

bool HG::Core::Behaviour::isEnabled() const
{
    return m_enabled;
}

void HG::Core::Behaviour::setEnabled(bool value)
{
    m_enabled = value;
}

const HG::Core::Input *HG::Core::Behaviour::input() const
{
    return scene()->application()->input();
}

void HG::Core::Behaviour::update()
{
    onUpdate();
}

void HG::Core::Behaviour::start()
{
    onStart();
}

void HG::Core::Behaviour::onStart()
{

}

void HG::Core::Behaviour::onUpdate()
{

}

void HG::Core::Behaviour::onFixedUpdate()
{

}

HG::Core::GameObject* HG::Core::Behaviour::gameObject() const
{
    return m_parent;
}

void HG::Core::Behaviour::setParentGameObject(HG::Core::GameObject* ptr)
{
    m_parent = ptr;
}

HG::Core::Scene* HG::Core::Behaviour::scene() const
{
    if (m_parent == nullptr)
    {
        return nullptr;
    }

    return m_parent->scene();
}

void HG::Core::Behaviour::addProperty(HG::Core::Behaviour::Property property)
{
    // todo: Add checking if property with this name already exists
    m_properties.emplace_back(property.name(), property);
}

void HG::Core::Behaviour::getProperties(std::vector<HG::Core::Behaviour::Property>& container) const
{
    container.reserve(m_properties.size());

    for (auto&& [name, prop] : m_properties)
    {
        container.push_back(prop);
    }
}

std::vector<HG::Core::Behaviour::Property> HG::Core::Behaviour::getProperties() const
{
    std::vector<Property> container;

    getProperties(container);

    return container;
}
