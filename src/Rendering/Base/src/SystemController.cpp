#include <Application.hpp>
#include "SystemController.hpp"


RENDERING_BASE_MODULE_NS::SystemController::SystemController(CORE_MODULE_NS::Application *application) :
    m_application(application)
{

}

::CORE_MODULE_NS::Application *RENDERING_BASE_MODULE_NS::SystemController::application() const
{
    return m_application;
}
