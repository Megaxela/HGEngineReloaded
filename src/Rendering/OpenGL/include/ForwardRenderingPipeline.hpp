#pragma once

#include <RenderingPipeline.hpp>
#include <Scene.hpp>

#include <Behaviours/Mesh.hpp>
#include <gl/program.hpp>
#include <gl/vertex_array.hpp>
#include <Behaviours/Sprite.hpp>
#include "GizmosRenderer.hpp"

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
         * @brief External data implementation for mesh rendering behaviour
         */
        class MeshData : public ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::ExternalData
        {
        public:

            gl::vertex_array VAO;
            gl::buffer VBO;
            gl::buffer EBO;
        };

        /**
         * @brief External data implementation for textures.
         */
        class TextureData : public ::RENDERING_BASE_MODULE_NS::Texture::TextureExternalData
        {
        public:

            gl::texture_2d Texture;
        };

        class CubeMapTextureData : public ::RENDERING_BASE_MODULE_NS::CubeMapTexture::CubeMapTextureExternalData
        {
        public:

            gl::cubemap_texture Texture;
        };

        /**
         * @brief External data implementation for shaders.
         */
        class ShaderData : public ::RENDERING_BASE_MODULE_NS::Shader::ShaderExternalData
        {
        public:

            gl::program Program;
        };

        /**
         * @brief Constructor.
         */
        explicit ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Destructor.
         */
        ~ForwardRenderingPipeline() override;

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

        /**
         * @brief Method for processing sprite renderer on
         * gameobject.
         * @param gameObject Owner of sprite renderer.
         * @param spriteBehaviour Actual rendering behaviour.
         */
        void renderSprite(
            ::CORE_MODULE_NS::GameObject* gameObject,
            ::RENDERING_BASE_MODULE_NS::Behaviours::Sprite* spriteBehaviour
        );

        /**
         * @brief Method for setting program uniform value.
         * @param program Pointer to program.
         * @param name Uniform name.
         * @param value Value.
         */
        void setShaderUniform(gl::program* program,
                              const std::string& name,
                              const ::RENDERING_BASE_MODULE_NS::Material::Value& value);

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

        bool initFallbackShader();

        bool initSpriteShader();

        GLuint m_textureNumber;

        std::vector<::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_behavioursCache;

        std::multimap<float, ::RENDERING_BASE_MODULE_NS::RenderBehaviour*> m_sortedBehaviours;

        // Fallback mesh program
        ::RENDERING_BASE_MODULE_NS::Shader m_meshFallback;

        // Shader for sprite rendering
        ::RENDERING_BASE_MODULE_NS::Shader m_spriteShader;

        // Sprite mesh
        MeshData* m_spriteData;

        // Gizmos rendering object
        GizmosRenderer m_gizmosRenderer;
    };
}

