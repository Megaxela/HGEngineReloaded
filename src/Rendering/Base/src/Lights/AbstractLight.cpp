// HG::Core
#include <HG/Core/BuildProperties.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/AbstractLight.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Rendering::Base
{
std::vector<AbstractLight*> AbstractLight::m_lights;

AbstractLight::AbstractLight(AbstractLight::Type type) : m_type(type), m_color(1.0f, 1.0f, 1.0f)
{
}

AbstractLight::~AbstractLight()
{
    // Removing from db

    AbstractLight::m_lights.erase(std::find(AbstractLight::m_lights.begin(), AbstractLight::m_lights.end(), this),
                                  AbstractLight::m_lights.end());
}

AbstractLight::Type AbstractLight::lightType() const
{
    return m_type;
}

void AbstractLight::setColor(const HG::Utils::Color& color)
{
    m_color = color;
}

HG::Utils::Color AbstractLight::color() const
{
    return m_color;
}

void AbstractLight::onStart()
{
    if constexpr (HG::Core::BuildProperties::isDebug())
    {
        if (std::find(AbstractLight::m_lights.begin(), AbstractLight::m_lights.end(), this) !=
            AbstractLight::m_lights.end())
        {
            HGError() << "Trying to add light to global light system, several times.";
        }
    }

    AbstractLight::m_lights.push_back(this);
}

const std::vector<AbstractLight*>& AbstractLight::totalLights()
{
    return m_lights;
}
} // namespace HG::Rendering::Base
