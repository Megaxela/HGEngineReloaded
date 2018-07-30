#include "PhysicsController.hpp"

PHYSICS_BASE_MODULE_NS::PhysicsController::PhysicsController(::CORE_MODULE_NS::Application *application) :
    m_parent(application)
{

}

::CORE_MODULE_NS::Application *PHYSICS_BASE_MODULE_NS::PhysicsController::application() const
{
    return m_parent;
}
