// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/PointLight.hpp>

HG::Rendering::Base::Lights::PointLight::PointLight() :
    AbstractLight(Type::Point),
    m_constant(2),
    m_linear(2),
    m_quadratic(2)
{

}

void HG::Rendering::Base::Lights::PointLight::setConstant(float value)
{
    m_constant = value;
}

float HG::Rendering::Base::Lights::PointLight::constant() const
{
    return m_constant;
}

void HG::Rendering::Base::Lights::PointLight::setLinear(float value)
{
    m_linear = value;
}

float HG::Rendering::Base::Lights::PointLight::linear() const
{
    return m_linear;
}

void HG::Rendering::Base::Lights::PointLight::setQuadratic(float value)
{
    m_quadratic = value;
}

float HG::Rendering::Base::Lights::PointLight::quadratic() const
{
    return m_quadratic;
}
