// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp>
#include <HG/Rendering/Base/RenderBehaviour.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Rendering::Base
{
RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    m_parentApplication(application),
    m_currentRenderTarget(nullptr),
    m_renderDataProcessor(),
    m_renderOverride(nullptr)
{
}

RenderingPipeline::~RenderingPipeline()
{
    for (auto&& processor : m_renderDataProcessor)
    {
        delete processor.second;
    }
}

bool RenderingPipeline::init()
{
    if (!m_parentApplication->systemController()->createWindow(800, 600, m_parentApplication->title()))
    {
        HGError("Can't create window, see errors above");
        return false;
    }

    if (!m_parentApplication->renderer())
    {
        HGError("No parent renderer, can't set default render target");
        return false;
    }

    return true;
}

void RenderingPipeline::deinit()
{
    if (m_parentApplication == nullptr)
    {
        HGError("No parent application set");
    }

    if (m_parentApplication->systemController() == nullptr)
    {
        HGError("No SystemController set in rendering pipeline");
    }

    m_parentApplication->systemController()->deinit();

    m_parentApplication->systemController()->closeWindow();
}

HG::Core::Application* RenderingPipeline::application() const
{
    return m_parentApplication;
}

bool RenderingPipeline::setup(RenderData* data, bool guarantee)
{
    BENCH(fmt::format("Setup of resource {}", data->dataType()));

    if (data->dataType() == RenderBehaviour::RenderDataId)
    {
        return setupRenderBehaviour(dynamic_cast<RenderBehaviour*>(data), guarantee);
    }

    // Search for data processor.
    auto processorIterator = m_renderDataProcessor.find(data->dataType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        HGError("Can't setup render data with {} type, no processor", data->dataType());
        return false;
    }

    return processorIterator->second->setup(data, guarantee);
}

bool RenderingPipeline::needSetup(RenderData* data)
{
    BENCH(fmt::format("Checking is setup required for {}", data->dataType()));
    if (data->dataType() == RenderBehaviour::RenderDataId)
    {
        return needSetupRenderBehaviour(dynamic_cast<RenderBehaviour*>(data));
    }

    // Search for data processor.
    auto processorIterator = m_renderDataProcessor.find(data->dataType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        HGError("Can't check is render data with type {} requires to be set up, no processor", data->dataType());
        return false;
    }

    return processorIterator->second->needSetup(data);
}

bool RenderingPipeline::setupRenderBehaviour(RenderBehaviour* behaviour, bool guarantee)
{
    BENCH(fmt::format("Setup of rendering behaviour {}", behaviour->renderBehaviourType()));
    auto processorIterator = m_renderDataProcessor.find(behaviour->renderBehaviourType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        HGError("Can't setup rendering behaviour's render data with {} type, no processor",
                behaviour->renderBehaviourType());
        return false;
    }

    return processorIterator->second->setup(behaviour, guarantee);
}

bool RenderingPipeline::needSetupRenderBehaviour(RenderBehaviour* behaviour)
{
    BENCH(fmt::format("Is setup required for rendering behaviour {}", behaviour->renderBehaviourType()));
    auto processorIterator = m_renderDataProcessor.find(behaviour->renderBehaviourType());

    if (processorIterator == m_renderDataProcessor.end())
    {
        HGError("Can't check rendering behaviour's setup requirement with {} type, no processor",
                behaviour->renderBehaviourType());
        return false;
    }

    return processorIterator->second->needSetup(behaviour);
}

RenderingPipeline* RenderingPipeline::addRenderDataProcessor(AbstractRenderDataProcessor* processor)
{
    processor->setRenderingPipeline(this);
    m_renderDataProcessor[processor->getTarget()] = processor;

    return this;
}

void RenderingPipeline::setRenderTarget(RenderTarget* target)
{
    m_currentRenderTarget = target;
}

RenderTarget* RenderingPipeline::renderTarget() const
{
    return m_currentRenderTarget;
}

void RenderingPipeline::setRenderOverride(RenderOverride* renderOverride)
{
    m_renderOverride = renderOverride;
}

RenderOverride* RenderingPipeline::renderOverride() const
{
    return m_renderOverride;
}
} // namespace HG::Rendering::Base
