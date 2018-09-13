// HG::Physics::Base
#include <PhysicsController.hpp>

HG::Physics::Base::PhysicsController::PhysicsController(HG::Core::Application *application) :
    m_parent(application)
{

}

HG::Core::Application *HG::Physics::Base::PhysicsController::application() const
{
    return m_parent;
}
