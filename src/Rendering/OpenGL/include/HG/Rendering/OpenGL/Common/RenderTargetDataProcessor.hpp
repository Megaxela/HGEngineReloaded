#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/AbstractRenderDataProcessor.hpp> // Required for inheritance


namespace HG::Rendering::OpenGL::Common
{
    class RenderTargetData;

    /**
     * @brief Class, that describes render target processor.
     */
    class RenderTargetDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:
        /**
         * @brief Constructor.
         */
        RenderTargetDataProcessor();

        /**
         * @brief Method for setting up render target
         * objects. Also this method performs default
         * render target setup.
         * @param data Pointer to render data.
         * @return Success.
         */
        bool setup(HG::Rendering::Base::RenderData* data, bool guarantee) override;

        std::size_t getTarget() override;

        bool needSetup(HG::Rendering::Base::RenderData* data) override;

    };
}

