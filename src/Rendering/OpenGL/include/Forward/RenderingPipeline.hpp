#pragma once

#include <RenderingPipeline.hpp>
#include <Scene.hpp>

#include <Behaviours/Mesh.hpp>
#include <gl/program.hpp>
#include <gl/vertex_array.hpp>
#include <Behaviours/Sprite.hpp>
#include <Common/MeshData.hpp>
#include "GizmosRenderer.hpp"
#include "AbstractRenderer.hpp"

namespace OGL_RENDERING_MODULE_NS::Forward
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
    class RenderingPipeline : public RENDERING_BASE_MODULE_NS::RenderingPipeline
    {
    public:

        class CubeMapTextureData : public ::RENDERING_BASE_MODULE_NS::CubeMapTexture::CubeMapTextureExternalData
        {
        public:

            gl::cubemap_texture Texture;
        };

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Destructor.
         */
        ~RenderingPipeline() override;

        /**
         * @brief Method for adding renderer to pipeline.
         * @param renderer Pointer to renderer.
         * @return Pointer to this pipeline.
         */
        RenderingPipeline* addRenderer(AbstractRenderer* renderer);

        /**
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects) override;

        /**
         * @brief Method for setting up behaviours for forward rendering.
         * @param behaviour Rendering behaviour.
         */
        void setup(::RENDERING_BASE_MODULE_NS::RenderBehaviour *behaviour) override;

        /**
         * @brief Method for setting up textures for forward rendering.
         * @param texture Texture.
         */
        void setup(::RENDERING_BASE_MODULE_NS::Texture* texture) override;

        /**
         * @brief Method for setting up shaders for forward rendering.
         * @param shader Shader.
         */
        void setup(::RENDERING_BASE_MODULE_NS::Shader* shader) override;

        /**
         * @brief Method for setting up cube maps for forward rendering.
         * @param texture Cube map texture.
         */
        void setup(::RENDERING_BASE_MODULE_NS::CubeMapTexture* texture) override;

        /**
         * @brief Init method.
         * @return Init success.
         */
        bool init() override;

    private:

        /**
         * @brief Method for processing game objects and it's behaviours.
         * @param objects Objects.
         */
        void proceedGameObjects(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects);

        // Setup methods
        void setupMesh(::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* behaviour);

        /**
         * @brief Method for converting texture enum to
         * gl enum filtering mode.
         * @param filter Texture enum.
         * @return GL enum.
         */
        GLuint getFilter(::RENDERING_BASE_MODULE_NS::Texture::Filtering filter);

        /**
         * @brief Method for converting texture enum to
         * gl enum wrapping mode.
         * @param wrapping Texture enum.
         * @return GL enum.
         */
        GLuint getWrapping(::RENDERING_BASE_MODULE_NS::Texture::Wrapping wrapping);

        /**
         * @brief Method for setting up one of cube side.
         * @param surface Surface for setting up.
         * @param texture GL texture.
         * @param side GL side.
         */
        void setupCubeMapSide(const ::UTILS_MODULE_NS::SurfacePtr& surface,
                              gl::cubemap_texture& texture,
                              GLuint side);

        std::vector<::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_behavioursCache;

        std::multimap<float, ::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_sortedBehaviours;

        // Gizmos rendering object
        GizmosRenderer m_gizmosRenderer;

        std::map<
            std::size_t,
            AbstractRenderer*
        > m_renderers;
    };
}

