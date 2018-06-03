#include "Lights/PointLight.hpp"

RENDERING_BASE_MODULE_NS::Lights::PointLight::PointLight() :
    AbstractLight(Type::Point),
    m_constant(2),
    m_linear(2),
    m_quadratic(2)
{

}

void RENDERING_BASE_MODULE_NS::Lights::PointLight::setConstant(float value)
{
    m_constant = value;
}

float RENDERING_BASE_MODULE_NS::Lights::PointLight::constant() const
{
    return m_constant;
}

void RENDERING_BASE_MODULE_NS::Lights::PointLight::setLinear(float value)
{
    m_linear = value;
}

float RENDERING_BASE_MODULE_NS::Lights::PointLight::linear() const
{
    return m_linear;
}

void RENDERING_BASE_MODULE_NS::Lights::PointLight::setQuadratic(float value)
{
    m_quadratic = value;
}

float RENDERING_BASE_MODULE_NS::Lights::PointLight::quadratic() const
{
    return m_quadratic;
}
