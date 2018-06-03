#include "Lights/PointLight.hpp"

RENDERING_BASE_MODULE_NS::Lights::PointLight::PointLight() :
    AbstractLight(Type::Point),
    m_radius(2)
{

}

float RENDERING_BASE_MODULE_NS::Lights::PointLight::radius() const
{
    return m_radius;
}

void RENDERING_BASE_MODULE_NS::Lights::PointLight::setRadius(float rad)
{
    m_radius = rad;
}
