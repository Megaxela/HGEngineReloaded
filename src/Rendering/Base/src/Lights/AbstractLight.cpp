// HG::Core
#include <HG/Core/BuildProperties.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/AbstractLight.hpp>

// ALogger
#include <CurrentLogger.hpp>

std::vector<HG::Rendering::Base::AbstractLight*> HG::Rendering::Base::AbstractLight::m_lights;

HG::Rendering::Base::AbstractLight::AbstractLight(HG::Rendering::Base::AbstractLight::Type type) :
    m_type(type),
    m_color(1.0f, 1.0f, 1.0f)
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

void HG::Rendering::Base::AbstractLight::setColor(const HG::Utils::Color& color)
{
    m_color = color;
}

HG::Utils::Color HG::Rendering::Base::AbstractLight::color() const
{
    return m_color;
}

void HG::Rendering::Base::AbstractLight::onStart()
{
    if constexpr (HG::Core::BuildProperties::isDebug())
    {
        if (std::find(
            AbstractLight::m_lights.begin(),
            AbstractLight::m_lights.end(),
            this
        ) != AbstractLight::m_lights.end())
        {
            Error() << "Trying to add light to global light system, several times.";
        }
    }

    AbstractLight::m_lights.push_back(this);
}

const std::vector<HG::Rendering::Base::AbstractLight *> &HG::Rendering::Base::AbstractLight::totalLights()
{
    return m_lights;
}
