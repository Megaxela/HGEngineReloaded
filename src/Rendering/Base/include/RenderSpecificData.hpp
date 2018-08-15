#pragma once

namespace HG::Rendering::Base
{
    /**
     * @brief Structure, that describes base for
     * any render specific data, that can be set to
     * any RenderData object.
     */
    struct RenderSpecificData
    {
        virtual ~RenderSpecificData() = default;
    };
}