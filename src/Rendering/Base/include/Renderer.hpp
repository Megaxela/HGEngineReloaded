#pragma once

#include "RenderingPipeline.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * layer between rendering pipeline and
     * user behaviour rendering system.
     */
    class Renderer
    {
    public:

        /**
         * @brief Default constructor.
         * @return
         */
        Renderer();

        /**
         * @brief Destructor.
         */
        ~Renderer();

        /**
         * @brief Method for setting pipeline
         * object.
         * @tparam PipelineType Type of pipeline object.
         */
        template<typename PipelineType>
        void setPipeline()
        {
            m_pipeline = new PipelineType();
        }

        /**
         * @brief Perform rendering finally.
         */
        void render();

    private:
        RenderingPipeline* m_pipeline;

    };
}

