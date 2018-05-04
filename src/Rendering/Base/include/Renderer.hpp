#pragma once

#include <Scene.hpp>
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
        typename std::enable_if<
            std::is_base_of<RenderingPipeline, PipelineType>::value
        >::type
        setPipeline()
        {
            m_pipeline = new PipelineType();
        }

        /**
         * @brief Method for getting pipeline object.
         * @return Pointer to pipeline object.
         */
        RenderingPipeline* pipeline();

        /**
         * @brief Method for initializing pipeline.
         */
        bool init();

        /**
         * @brief Perform rendering finally.
         */
        void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& gameObjects);

    private:
        ::RENDERING_BASE_MODULE_NS::RenderingPipeline* m_pipeline;

    };
}

