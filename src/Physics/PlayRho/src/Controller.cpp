#include <chrono>
#include "Controller.hpp"

PLAYRHO_PHYSICS_MODULE_NS::Controller::Controller(::CORE_MODULE_NS::Application *parent) :
    PhysicsController(parent),
    m_stepConfiguration(),
    m_world()
{

}

playrho::d2::World *PLAYRHO_PHYSICS_MODULE_NS::Controller::world()
{
    return &m_world;
}

void PLAYRHO_PHYSICS_MODULE_NS::Controller::tick(std::chrono::microseconds deltaTime)
{
//    std::cout << "Time: " << deltaTime.count() << std::endl;

    m_stepConfiguration.SetTime(
//        deltaTime.count() / 1000000.0f
        playrho::Time(1.0 / 60)
    );

    m_world.Step(m_stepConfiguration);
}
