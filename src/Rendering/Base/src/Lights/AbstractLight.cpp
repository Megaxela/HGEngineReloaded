#include "Lights/AbstractLight.hpp"

std::vector<RENDERING_BASE_MODULE_NS::AbstractLight*> RENDERING_BASE_MODULE_NS::AbstractLight::m_lights;

RENDERING_BASE_MODULE_NS::AbstractLight::AbstractLight(RENDERING_BASE_MODULE_NS::AbstractLight::Type type) :
    m_type(type),
    m_ambient(),
    m_diffuse(),
    m_specular()
{

}

RENDERING_BASE_MODULE_NS::AbstractLight::~AbstractLight()
{
    // Removing from db

    AbstractLight::m_lights.erase(
        std::find(
            AbstractLight::m_lights.begin(),
            AbstractLight::m_lights.end(),
            this
        ),
        AbstractLight::m_lights.end()
    );
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
#ifndef NDEBUG

    if (std::find(
        AbstractLight::m_lights.begin(),
        AbstractLight::m_lights.end(),
        this
    ) != AbstractLight::m_lights.end())
    {
        Error() << "Trying to add light to global light system, several times.";
    }

#endif

    AbstractLight::m_lights.push_back(this);
}
