#pragma once

// C++ STL
#include <cstdlib>

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class RenderData;
    class RenderingPipeline;

    /**
     * @brief Class, that describes abstract processor of
     * render data processor.
     */
    class AbstractRenderDataProcessor
    {
    public:

        /**
         * @brief Constructor.
         */
        AbstractRenderDataProcessor();

        /**
         * @brief Default virtual destructor.
         */
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
         * @param guarantee If true - processor has to wait for
         * all async calls,
         * @return Setup success.
         */
        virtual bool setup(HG::Rendering::Base::RenderData* data, bool guarantee) = 0;

        /**
         * @brief Method for checking is render data needs
         * to set up.
         * @param data Pointer to render data.
         * @return Is render data required to be set up.
         */
        virtual bool needSetup(HG::Rendering::Base::RenderData* data) = 0;

        /**
         * @brief Method for setting parent rendering pipeline.
         * @param pipeline Pointer to parent rendering pipeline.
         */
        void setRenderingPipeline(HG::Rendering::Base::RenderingPipeline* pipeline);

        /**
         * @brief Method for getting parent rendering pipeline.
         * @return Parent rendering pipeline.
         */
        HG::Rendering::Base::RenderingPipeline* renderingPipeline() const;

        /**
         * @brief Method for getting parent application (it will be taked
         * from rendering pipeline)
         * @return Pointer to parent applicaiton.
         */
        HG::Core::Application* application() const;

    private:

        HG::Rendering::Base::RenderingPipeline* m_pipeline;

    };
}

