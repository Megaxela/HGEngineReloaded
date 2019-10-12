// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Scene.hpp>

namespace HG::Core
{
Behaviour::Behaviour(Type t) : m_type(t), m_enabled(true), m_parent(nullptr), m_properties()
{
}

Behaviour::~Behaviour()
{
    if (m_parent)
    {
        m_parent->removeBehaviour(this);
        m_parent = nullptr;
    }
}

Behaviour::Type Behaviour::type() const
{
    return m_type;
}

bool Behaviour::isEnabled() const
{
    return m_enabled;
}

void Behaviour::setEnabled(bool value)
{
    m_enabled = value;
}

const Input* Behaviour::input() const
{
    if (scene() == nullptr || scene()->application() == nullptr)
    {
        return nullptr;
    }

    return scene()->application()->input();
}

void Behaviour::update()
{
    onUpdate();
}

void Behaviour::start()
{
    onStart();
}

void Behaviour::onStart()
{
}

void Behaviour::onUpdate()
{
}

void Behaviour::onFixedUpdate()
{
}

GameObject* Behaviour::gameObject() const
{
    return m_parent;
}

void Behaviour::setParentGameObject(GameObject* ptr)
{
    m_parent = ptr;
}

Scene* Behaviour::scene() const
{
    if (m_parent == nullptr)
    {
        return nullptr;
    }

    return m_parent->scene();
}

void Behaviour::addProperty(Behaviour::Property property)
{
    // todo: Add checking if property with this name already exists
    m_properties.emplace_back(property.name(), property);
}

void Behaviour::getProperties(std::vector<Behaviour::Property>& container) const
{
    container.reserve(m_properties.size());

    for (auto&& [name, prop] : m_properties)
    {
        container.push_back(prop);
    }
}

std::vector<Behaviour::Property> Behaviour::getProperties() const
{
    std::vector<Property> container;

    getProperties(container);

    return container;
}
} // namespace HG::Core
