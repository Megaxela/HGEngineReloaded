#include <CurrentLogger.hpp>
#include "RenderingPipeline.hpp"

RENDERING_BASE_MODULE_NS::RenderingPipeline::RenderingPipeline() :
    m_systemController(nullptr)
{

}

RENDERING_BASE_MODULE_NS::RenderingPipeline::~RenderingPipeline()
{
    delete m_systemController;
}

RENDERING_BASE_MODULE_NS::SystemController* HG::Rendering::Base::RenderingPipeline::systemController() const
{
    return m_systemController;
}

bool RENDERING_BASE_MODULE_NS::RenderingPipeline::init()
{
    if (m_systemController == nullptr)
    {
        Error() << "No SystemController set in rendering pipeline.";
        return false;
    }

    if (!m_systemController->init())
    {
        Error() << "Can't init rendering pipeline by system controller. See errors above.";
        return false;
    }

    if (!m_systemController->createWindow(800, 600, "place_name_here"))
    {
        Error() << "Can't create window. See errors above.";
        return false;
    }

    return true;
}
