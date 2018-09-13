// HG::Rendering::Base
#include <SystemController.hpp>


HG::Rendering::Base::SystemController::SystemController(HG::Core::Application *application) :
    m_application(application)
{

}

HG::Core::Application *HG::Rendering::Base::SystemController::application() const
{
    return m_application;
}
