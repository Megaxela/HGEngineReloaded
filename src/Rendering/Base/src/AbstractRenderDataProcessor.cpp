// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>

HG::Rendering::Base::AbstractRenderDataProcessor::AbstractRenderDataProcessor() :
    m_pipeline(nullptr)
{

}

void HG::Rendering::Base::AbstractRenderDataProcessor::setRenderingPipeline(HG::Rendering::Base::RenderingPipeline* pipeline)
{
    m_pipeline = pipeline;
}

HG::Rendering::Base::RenderingPipeline *HG::Rendering::Base::AbstractRenderDataProcessor::renderingPipeline() const
{
    return m_pipeline;
}

HG::Core::Application *HG::Rendering::Base::AbstractRenderDataProcessor::application() const
{
    if (m_pipeline == nullptr)
    {
        return nullptr;
    }

    return m_pipeline->application();
}
