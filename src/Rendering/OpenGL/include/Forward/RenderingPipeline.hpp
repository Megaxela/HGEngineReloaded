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

namespace HG::Rendering::OpenGL::Forward
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
    class RenderingPipeline : public HG::Rendering::Base::RenderingPipeline
    {
    public:

        class CubeMapTextureData : public ::HG::Rendering::Base::CubeMapTexture::CubeMapTextureExternalData
        {
        public:

            gl::cubemap_texture Texture;
        };

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(::HG::Core::Application* application);

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
        void render(const ::HG::Core::Scene::GameObjectsContainer& objects) override;

        /**
         * @brief Method for setting up behaviours for forward rendering.
         * @param behaviour Rendering behaviour.
         */
        void setup(::HG::Rendering::Base::RenderBehaviour *behaviour) override;

        /**
         * @brief Method for setting up textures for forward rendering.
         * @param texture Texture.
         */
        void setup(::HG::Rendering::Base::Texture* texture) override;

        /**
         * @brief Method for setting up shaders for forward rendering.
         * @param shader Shader.
         */
        void setup(::HG::Rendering::Base::Shader* shader) override;

        /**
         * @brief Method for setting up cube maps for forward rendering.
         * @param texture Cube map texture.
         */
        void setup(::HG::Rendering::Base::CubeMapTexture* texture) override;

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
        void proceedGameObjects(const ::HG::Core::Scene::GameObjectsContainer& objects);

        // Setup methods
        void setupMesh(::HG::Rendering::Base::Behaviours::Mesh* behaviour);

        /**
         * @brief Method for converting texture enum to
         * gl enum filtering mode.
         * @param filter Texture enum.
         * @return GL enum.
         */
        GLuint getFilter(::HG::Rendering::Base::Texture::Filtering filter);

        /**
         * @brief Method for converting texture enum to
         * gl enum wrapping mode.
         * @param wrapping Texture enum.
         * @return GL enum.
         */
        GLuint getWrapping(::HG::Rendering::Base::Texture::Wrapping wrapping);

        /**
         * @brief Method for setting up one of cube side.
         * @param surface Surface for setting up.
         * @param texture GL texture.
         * @param side GL side.
         */
        void setupCubeMapSide(const ::HG::Utils::SurfacePtr& surface,
                              gl::cubemap_texture& texture,
                              GLuint side);

        std::vector<::HG::Rendering::Base::RenderBehaviour*> m_behavioursCache;

        std::multimap<float, ::HG::Rendering::Base::RenderBehaviour*> m_sortedBehaviours;

        // Gizmos rendering object
        GizmosRenderer m_gizmosRenderer;

        std::map<
            std::size_t,
            AbstractRenderer*
        > m_renderers;
    };
}

