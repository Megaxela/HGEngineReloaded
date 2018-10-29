#pragma once

// HG::Core
#include <HG/Core/Scene.hpp>
#include <HG/Core/Application.hpp>
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/TimeStatistics.hpp>

class LocalRotationBehaviour : public HG::Core::Behaviour
{
public:

    explicit LocalRotationBehaviour(float speed = 1.0f) :
        m_speed(speed)
    {}

protected:
    void onUpdate() override
    {

        auto dt = scene()
                      ->application()
                      ->timeStatistics()
                      ->lastFrameDeltaTime().count() / 1000000.0 * m_speed;

        gameObject()
            ->transform()
            ->setLocalRotation(
                gameObject()
                    ->transform()
                    ->localRotation() *
                glm::quat(glm::vec3(0, glm::radians(22.5f) * dt, 0))
            );
    }

private:

    float m_speed;
};