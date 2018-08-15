#pragma once

#include <AbstractRenderDataProcessor.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief Class, that describes shader data processor.
     */
    class ShaderDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:

        bool setup(HG::Rendering::Base::RenderData* data) override;

        std::size_t getTarget() override;

    };
}

