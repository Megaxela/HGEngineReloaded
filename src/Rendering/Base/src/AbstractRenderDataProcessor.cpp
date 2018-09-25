// HG::Rendering::Base
#include <AbstractRenderDataProcessor.hpp>

HG::Rendering::Base::AbstractRenderDataProcessor::AbstractRenderDataProcessor() :
    m_pipeline(nullptr)
{

}

void HG::Rendering::Base::AbstractRenderDataProcessor::setRenderingPipeline(HG::Rendering::Base::RenderingPipeline* pipeline)
{
    m_pipeline = pipeline;
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::AbstractRenderDataProcessor::renderingPipeline()
{
    return m_pipeline;
}
