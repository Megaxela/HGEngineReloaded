#pragma once

#include <RenderingPipeline.hpp>
#include <Scene.hpp>

namespace OGL_RENDERING_MODULE_NS
{
    /**
     * @brief Class, that describes OpenGL default
     * forward rendering pipeline.
     */
    class ForwardRenderingPipeline : public RENDERING_BASE_MODULE_NS::RenderingPipeline
    {
    public:
        /**
         * @brief Constructor.
         */
        ForwardRenderingPipeline();

        /**
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const HG::Core::Scene::GameObjectsContainer& objects) override;

    private:


    };
}

