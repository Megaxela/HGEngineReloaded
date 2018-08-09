#include "Lights/AbstractLight.hpp"

std::vector<HG::Rendering::Base::AbstractLight*> HG::Rendering::Base::AbstractLight::m_lights;

HG::Rendering::Base::AbstractLight::AbstractLight(HG::Rendering::Base::AbstractLight::Type type) :
    m_type(type),
    m_specular(1.0f, 1.0f, 1.0f),
    m_ambient(0.2f, 0.2f, 0.2f),
    m_diffuse(1.0f, 1.0f, 1.0f)
{

}

HG::Rendering::Base::AbstractLight::~AbstractLight()
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

HG::Rendering::Base::AbstractLight::Type HG::Rendering::Base::AbstractLight::type()
{
    return m_type;
}

void HG::Rendering::Base::AbstractLight::setSpecular(const HG::Utils::Color& color)
{
    m_specular = color;
}

HG::Utils::Color HG::Rendering::Base::AbstractLight::specular() const
{
    return m_specular;
}

void HG::Rendering::Base::AbstractLight::setAmbient(const HG::Utils::Color& color)
{
    m_ambient = color;
}

HG::Utils::Color HG::Rendering::Base::AbstractLight::ambient() const
{
    return m_ambient;
}

void HG::Rendering::Base::AbstractLight::setDiffuse(const HG::Utils::Color& color)
{
    m_diffuse = color;
}

HG::Utils::Color HG::Rendering::Base::AbstractLight::diffuse() const
{
    return m_diffuse;
}

void HG::Rendering::Base::AbstractLight::onStart()
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

const std::vector<HG::Rendering::Base::AbstractLight *> &HG::Rendering::Base::AbstractLight::totalLights()
{
    return m_lights;
}
