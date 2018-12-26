#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp> // Required for inheritance

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief Class, that describes object for cube map object
     * setting up.
     */
    class CubeMapTextureDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:
        size_t getTarget() override;

        bool setup(HG::Rendering::Base::RenderData* data, bool guarantee) override;

        bool needSetup(HG::Rendering::Base::RenderData* data) override;
    };
}

