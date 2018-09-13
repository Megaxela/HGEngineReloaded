#pragma once

// C++ STL
#include <cstdlib>

namespace HG::Rendering::Base
{
    class RenderData;
    /**
     * @brief Class, that describes abstract processor of
     * render data processor.
     */
    class AbstractRenderDataProcessor
    {
    public:

        virtual ~AbstractRenderDataProcessor() = default;

        /**
         * @brief Method, that will be used by pipeline
         * to identify what behaviour type will be proceed by
         * implementation.
         * @return Behaviour type.
         */
        virtual std::size_t getTarget() = 0;

        /**
         * @brief Method, that will be used by pipeline
         * to setup render data.
         * @param data Pointer to render data.
         * @return Setup success.
         */
        virtual bool setup(HG::Rendering::Base::RenderData* data) = 0;
    };
}

