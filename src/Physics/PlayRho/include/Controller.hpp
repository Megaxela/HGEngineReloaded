#pragma once

#include <PlayRho/Dynamics/World.hpp>
#include <chrono>
#include <PlayRho/Dynamics/StepConf.hpp>
#include "PhysicsController.hpp"
#include "DebugDraw.hpp"
#include "DebugSettings.hpp"

namespace PLAYRHO_PHYSICS_MODULE_NS
{
    /**
     * @brief PlayRho physics controller object.
     */
    class Controller : public ::PHYSICS_BASE_MODULE_NS::PhysicsController
    {
    public:

        /**
         * @brief Constructor.
         * @param parent Pointer to parent application.
         */
        explicit Controller(::CORE_MODULE_NS::Application* parent);

        /**
         * @brief Method for getting playrho world.
         * @return Pointer to world object.
         */
        playrho::d2::World* world();

        void tick(std::chrono::microseconds deltaTime) override;


    private:

        DebugSettings m_settings;

        DebugDraw m_drawer;

        playrho::StepConf m_stepConfiguration;

        playrho::d2::World m_world;
    };
}


