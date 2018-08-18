#include <AbstractRenderDataProcessor.hpp>
#include <RenderingPipeline.hpp>
#include <SystemController.hpp>
#include <CurrentLogger.hpp>
#include <Application.hpp>
#include <RenderBehaviour.hpp>

HG::Rendering::Base::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    m_parentApplication(application)
{

}

HG::Rendering::Base::RenderingPipeline::~RenderingPipeline()
{
    for (auto&& processor : m_renderDataProcessor)
    {
        delete processor.second;
    }
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

bool HG::Rendering::Base::RenderingPipeline::setup(HG::Rendering::Base::RenderData* data)
{
    if (data->dataType() == HG::Rendering::Base::RenderBehaviour::DataId)
    {
        return setupRenderBehaviour(
            dynamic_cast<HG::Rendering::Base::RenderBehaviour*>(data)
        );
    }

    // Search for data processor.
    auto processorIterator = m_renderDataProcessor.find(data->dataType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't setup render data with " << data->dataType() << " type. No processor.";
        return false;
    }

    return processorIterator->second->setup(data);
}

bool HG::Rendering::Base::RenderingPipeline::setupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    auto processorIterator = m_renderDataProcessor.find(behaviour->renderBehaviourType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        Error() << "Can't setup rendering behaviour's render data with " << behaviour->renderBehaviourType() << " type. No processor.";
        return false;
    }

    return processorIterator->second->setup(behaviour);
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::RenderingPipeline::addRenderDataProcessor(HG::Rendering::Base::AbstractRenderDataProcessor* processor)
{
    m_renderDataProcessor[processor->getTarget()] = processor;

    return &(*this);
}
