#pragma once

// HG::Rendering::Base
#include <AbstractRenderDataProcessor.hpp> // Required for inheritance

namespace HG::Rendering::OpenGL::Common
{
    class CubeMapTextureDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:
        size_t getTarget() override;

        bool setup(HG::Rendering::Base::RenderData* data) override;
    };
}

