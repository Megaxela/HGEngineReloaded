// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/PointLight.hpp>

namespace HG::Rendering::Base::Lights
{
PointLight::PointLight() : AbstractLight(Type::Point), m_constant(2), m_linear(2), m_quadratic(2)
{
}

void PointLight::setConstant(float value)
{
    m_constant = value;
}

float PointLight::constant() const
{
    return m_constant;
}

void PointLight::setLinear(float value)
{
    m_linear = value;
}

float PointLight::linear() const
{
    return m_linear;
}

void PointLight::setQuadratic(float value)
{
    m_quadratic = value;
}

float PointLight::quadratic() const
{
    return m_quadratic;
}
} // namespace HG::Rendering::Base::Lights
