#include "Lights/AbstractLight.hpp"

RENDERING_BASE_MODULE_NS::AbstractLight::AbstractLight(RENDERING_BASE_MODULE_NS::AbstractLight::Type type) :
    m_type(type),
    m_ambient(),
    m_diffuse(),
    m_specular()
{

}

RENDERING_BASE_MODULE_NS::AbstractLight::~AbstractLight()
{

}

RENDERING_BASE_MODULE_NS::AbstractLight::Type RENDERING_BASE_MODULE_NS::AbstractLight::type()
{
    return m_type;
}

void RENDERING_BASE_MODULE_NS::AbstractLight::setSpecular(const UTILS_MODULE_NS::Color& color)
{
    m_specular = color;
}

UTILS_MODULE_NS::Color RENDERING_BASE_MODULE_NS::AbstractLight::specular() const
{
    return m_specular;
}

void RENDERING_BASE_MODULE_NS::AbstractLight::setAmbient(const UTILS_MODULE_NS::Color& color)
{
    m_ambient = color;
}

UTILS_MODULE_NS::Color RENDERING_BASE_MODULE_NS::AbstractLight::ambient() const
{
    return m_ambient;
}

void RENDERING_BASE_MODULE_NS::AbstractLight::setDiffuse(const UTILS_MODULE_NS::Color& color)
{
    m_diffuse = color;
}

UTILS_MODULE_NS::Color RENDERING_BASE_MODULE_NS::AbstractLight::diffuse() const
{
    return m_diffuse;
}

void RENDERING_BASE_MODULE_NS::AbstractLight::onStart()
{
    Behaviour::onStart();

    // todo: Add registration to lighting database
}
