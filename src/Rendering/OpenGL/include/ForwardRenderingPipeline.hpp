#pragma once

#include <RenderingPipeline.hpp>
#include <Scene.hpp>

#include <Behaviours/Mesh.hpp>
#include <gl/program.hpp>

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
        explicit ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects) override;

    private:

        /**
         * @brief Method for processing mesh renderer on
         * gameobject.
         * @param gameObject Owner of mesh renderer.
         * @param meshBehaviour Actual rendering behaviour.
         */
        void renderMesh(
            ::CORE_MODULE_NS::GameObject* gameObject,
            ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* meshBehaviour
        );

        std::vector<::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_behavoursCache;

        // Fallback mesh program
        gl::program m_meshFallback;
    };
}

