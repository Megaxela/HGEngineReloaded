#pragma once

#include <AbstractRenderDataProcessor.hpp>

namespace HG::Rendering::OpenGL::Common
{
    class CubeMapTextureDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:
        size_t getTarget() override;

        bool setup(HG::Rendering::Base::RenderData* data) override;
    };
}

