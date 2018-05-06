#include <CurrentLogger.hpp>
#include "RenderingPipeline.hpp"
#include <Application.hpp>

RENDERING_BASE_MODULE_NS::RenderingPipeline::RenderingPipeline(::CORE_MODULE_NS::Application* application) :
    m_parentApplication(application)
{

}

bool RENDERING_BASE_MODULE_NS::RenderingPipeline::init()
{
    if (m_parentApplication == nullptr)
    {
        Error() << "No parent application set.";
        return false;
    }

    if (m_parentApplication->systemController() == nullptr)
    {
        Error() << "No SystemController set in rendering pipeline.";
        return false;
    }

    if (!m_parentApplication->systemController()->init())
    {
        Error() << "Can't init rendering pipeline by system controller. See errors above.";
        return false;
    }

    if (!m_parentApplication->systemController()->createWindow(800, 600, "place_name_here"))
    {
        Error() << "Can't create window. See errors above.";
        return false;
    }

    return true;
}

::CORE_MODULE_NS::Application *RENDERING_BASE_MODULE_NS::RenderingPipeline::application() const
{
    return m_parentApplication;
}
