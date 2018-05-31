#pragma once

#include <RenderingPipeline.hpp>
#include <Scene.hpp>

#include <Behaviours/Mesh.hpp>
#include <gl/program.hpp>
#include <gl/vertex_array.hpp>

namespace OGL_RENDERING_MODULE_NS
{
    /**
     * @brief Class, that describes OpenGL default
     * forward rendering pipeline.
     *
     * Mesh shader attributes layout:
     * 0. vec3 position
     * 1. vec3 normal
     * 2. vec2 uv
     * 3. vec3 tangent
     * 4. vec3 bitangent
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

        /**
         * @brief Method for setting up behaviours for forward rendering.
         * @param behaviour
         */
        void setup(::RENDERING_BASE_MODULE_NS::RenderBehaviour *behaviour) override;

        /**
         * @brief Init method.
         * @return Init success.
         */
        bool init() override;

    private:

        /**
         * @brief External data implementation for mesh rendering behaviour
         */
        class MeshData : public ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::MeshExternalData
        {
        public:

            gl::vertex_array VAO;
            gl::buffer VBO;
            gl::buffer EBO;
        };

        // Setup methods
        void setupMesh(::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* behaviour);

        // Render methods
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

        std::vector<::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_behavioursCache;

        // Fallback mesh program
        gl::program m_meshFallback;
    };
}

