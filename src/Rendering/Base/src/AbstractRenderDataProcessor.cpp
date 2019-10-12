// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>

namespace HG::Rendering::Base
{
AbstractRenderDataProcessor::AbstractRenderDataProcessor() : m_pipeline(nullptr)
{
}

void AbstractRenderDataProcessor::setRenderingPipeline(RenderingPipeline* pipeline)
{
    m_pipeline = pipeline;
}

RenderingPipeline* AbstractRenderDataProcessor::renderingPipeline() const
{
    return m_pipeline;
}

HG::Core::Application* AbstractRenderDataProcessor::application() const
{
    if (m_pipeline == nullptr)
    {
        return nullptr;
    }

    return m_pipeline->application();
}
} // namespace HG::Rendering::Base
