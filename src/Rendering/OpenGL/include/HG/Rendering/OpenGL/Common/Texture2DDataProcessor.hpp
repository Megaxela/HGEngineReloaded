#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief Class, that describes 2d texture data processor
     */
    class Texture2DDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:

        bool setup(HG::Rendering::Base::RenderData* data) override;

        size_t getTarget() override;

        bool needSetup(HG::Rendering::Base::RenderData* data) override;
    };
}

