#pragma once

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/TimeStatistics.hpp>
#include <HG/Core/Transform.hpp>

class RotationBehaviour : public HG::Core::Behaviour
{
public:
    RotationBehaviour(glm::vec3 center, float radius) : m_radius(radius), m_center(center), m_radiansValue(0)
    {
    }

protected:
    void onUpdate() override
    {
        auto dt = scene()->application()->timeStatistics()->lastFrameDeltaTime().count() / 1000000.0;

        m_radiansValue += M_PI_2 * dt;

        gameObject()->transform()->setGlobalPosition(
            m_center + glm::vec3(std::cos(m_radiansValue) * m_radius, 0, std::sin(m_radiansValue) * m_radius));
    }

private:
    float m_radius;
    glm::vec3 m_center;

    double m_radiansValue;
};
