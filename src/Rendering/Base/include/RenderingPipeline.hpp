#pragma once

#include "SystemController.hpp"
#include "Scene.hpp"
#include "RenderBehaviour.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "CubeMapTexture.hpp"

namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describes
     * abstract rendering pipeline.
     */
    class RenderingPipeline
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Desturctor.
         */
        virtual ~RenderingPipeline() = default;

        /**
         * @brief Method for initializing pipeline.
         * By default it just initialize system controller
         * and creates window.
         */
        virtual bool init();

        /**
         * @brief Rendering method.
         * @param objects Objects.
         */
        virtual void render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects) = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        ::CORE_MODULE_NS::Application* application() const;

        /**
         * @brief Method for setting up render behaviour.
         * @param behaviour Pointer to render behaviour.
         */
        virtual void setup(RenderBehaviour* behaviour) = 0;

        /**
         * @brief Method for setting up textures for rendering.
         * @param texture Pointer to texture.
         */
        virtual void setup(Texture* texture) = 0;

        /**
         * @brief Method for setting up shaders for rendering.
         * @param shader Pointer to shader.
         */
        virtual void setup(Shader* shader) = 0;

        /**
         * @brief Method for setting up cube map textures for rendering.
         * @param texture Pointer to cube map.
         */
        virtual void setup(CubeMapTexture* texture) = 0;

    private:

        ::CORE_MODULE_NS::Application* m_parentApplication;
    };
}

