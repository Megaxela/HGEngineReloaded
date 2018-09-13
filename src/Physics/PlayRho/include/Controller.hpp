#pragma once

// C++ STL
#include <chrono>

// HG::Physics::Core
#include <PhysicsController.hpp> // Required for inheritance

// HG::Physics::PlayRho
#include <DebugSettings.hpp>
#include <DebugDraw.hpp>

// PlayRho
#include <PlayRho/Dynamics/StepConf.hpp>
#include <PlayRho/Dynamics/World.hpp>

namespace HG::Physics::PlayRho
{
    /**
     * @brief PlayRho physics controller object.
     */
    class Controller : public HG::Physics::Base::PhysicsController
    {
    public:

        /**
         * @brief Constructor.
         * @param parent Pointer to parent application.
         */
        explicit Controller(HG::Core::Application* parent);

        /**
         * @brief Method for getting playrho world.
         * @return Pointer to world object.
         */
        playrho::d2::World* world();

        /**
         * @brief Method for ticking physics engine for some time.
         * @param deltaTime Time in milliseconds.
         */
        void tick(std::chrono::microseconds deltaTime) override;

        /**
         * @brief Method for getting controller settings.
         * @return Pointer to settings.
         */
        DebugSettings* debugSettings();

        /**
         * @brief Method for getting step configuration.
         * @return Pointer to step conf.
         */
        playrho::StepConf* stepConfiguration();

    private:

        DebugSettings m_settings;

        DebugDraw m_drawer;

        playrho::StepConf m_stepConfiguration;

        playrho::d2::World m_world;
    };
}


