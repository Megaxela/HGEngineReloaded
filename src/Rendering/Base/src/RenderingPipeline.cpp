#include <RenderingPipeline.hpp>
#include <SystemController.hpp>
#include <CurrentLogger.hpp>
#include <Application.hpp>

HG::Rendering::Base::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    m_parentApplication(application)
{

}

bool HG::Rendering::Base::RenderingPipeline::init()
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

void HG::Rendering::Base::RenderingPipeline::deinit()
{
    if (m_parentApplication == nullptr)
    {
        Error() << "No parent application set.";
    }

    if (m_parentApplication->systemController() == nullptr)
    {
        Error() << "No SystemController set in rendering pipeline.";
    }

    m_parentApplication->systemController()->deinit();

    m_parentApplication->systemController()->closeWindow();
}

HG::Core::Application *HG::Rendering::Base::RenderingPipeline::application() const
{
    return m_parentApplication;
}
