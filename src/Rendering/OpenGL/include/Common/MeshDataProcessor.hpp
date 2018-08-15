#pragma once

#include "AbstractRenderDataProcessor.hpp"

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief Class, that describes mesh data processor.
     */
    class MeshDataProcessor : public HG::Rendering::Base::AbstractRenderDataProcessor
    {
    public:

        bool setup(HG::Rendering::Base::RenderData* data) override;

        std::size_t getTarget() override;
    };
}

