// HG::Rendering::Base
#include <HG/Rendering/Base/SystemController.hpp>

namespace HG::Rendering::Base
{
SystemController::SystemController(HG::Core::Application* application) : m_application(application)
{
}

HG::Core::Application* SystemController::application() const
{
    return m_application;
}
} // namespace HG::Rendering::Base
