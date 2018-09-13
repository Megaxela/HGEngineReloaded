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
protected:
    void onUpdate() override
    {

        auto dt = scene()
                      ->application()
                      ->timeStatistics()
                      ->lastFrameDeltaTime().count() / 1000000.0;

        gameObject()
            ->transform()
            ->setLocalRotation(
                gameObject()
                    ->transform()
                    ->localRotation() *
                glm::quat(glm::vec3(0, glm::radians(22.5f) * dt, 0))
            );
    }
};