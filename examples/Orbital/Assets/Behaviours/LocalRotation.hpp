#pragma once

// HG::Core
#include <Scene.hpp>
#include <Application.hpp>
#include <Behaviour.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <TimeStatistics.hpp>

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