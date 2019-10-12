// HG::Physics::Base
#include <HG/Physics/Base/PhysicsController.hpp>

namespace HG::Physics::Base
{
PhysicsController::PhysicsController(HG::Core::Application* application) : m_parent(application)
{
}

HG::Core::Application* PhysicsController::application() const
{
    return m_parent;
}
} // namespace HG::Physics::Base
