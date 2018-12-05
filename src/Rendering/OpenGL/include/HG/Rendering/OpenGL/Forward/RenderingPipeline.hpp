#pragma once

// C++ STL
#include <map>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderingPipeline.hpp> // Required for inheritance
#include <gl/vertex_array.hpp>

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;
}

namespace HG::Rendering::OpenGL
{
    class GizmosRenderer;
    class ImGuiRenderer;
    class BlitRenderer;
}

namespace HG::Rendering::OpenGL::Forward
{
    class AbstractRenderer;

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

        /**
         * @brief Constructor.
         */
        explicit RenderingPipeline(HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~RenderingPipeline() override;

        /**
         * @brief Method for adding renderer to pipeline.
         * @param renderer Pointer to renderer.
         * @return Pointer to this pipeline.
         */
        RenderingPipeline* addRenderer(HG::Rendering::OpenGL::Forward::AbstractRenderer* renderer);

        /**
         * @brief Renderbuffer clear implementation.
         * @param color Clear color.
         */
        void clear(HG::Utils::Color color) override;

        /**
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const std::vector<HG::Core::GameObject*>& objects) override;

        /**
         * @brief Method for rendering specified render
         * behaviour.
         * @param behaviour
         */
        bool render(HG::Rendering::Base::RenderBehaviour* behaviour) override;

        /**
         * @brief Method for blitting blit data to target rendertarget.
         * @param target Pointer to rendertarget.
         * @param blitData Pointer to blit data.
         */
        void blit(HG::Rendering::Base::RenderTarget *target, HG::Rendering::Base::BlitData *blitData) override;

        /**
         * @brief Init method.
         * @return Init success.
         */
        bool init() override;

        /**
         * @brief Deinit method.
         */
        void deinit() override;

        /**
         * @brief Method for setting current render target and
         * setting it up.
         * @param target Pointer to rendertarget.
         */
        void setRenderTarget(HG::Rendering::Base::RenderTarget* target) override;

        /**
         * @brief Method for getting texture region data.
         * @param texture Pointer to texture.
         * @param tl Top left.
         * @param br Bottom right.
         * @param data Pointer to data. It has to be >= (br.x - tl.x) * (br.y - tl.y) * 4 (RGBA)
         */
        void getTextureRegion(HG::Rendering::Base::Texture *texture, glm::ivec2 tl, glm::ivec2 br, uint8_t* data) override;

    private:

        /**
         * @brief Method for updating render target
         * according to override system.
         */
        void proceedRenderTargetOverride();

        /**
         * @brief Method for updating viewport if required.
         */
        void updateViewport();

        /**
         * @brief Method for processing game objects and it's behaviours.
         * @param objects Objects.
         */
        void proceedGameObjects(const std::vector<HG::Core::GameObject*>& objects);

        // Caching
        std::vector<HG::Rendering::Base::RenderBehaviour*> m_behavioursCache;
        std::multimap<float, HG::Rendering::Base::RenderBehaviour*> m_sortedBehaviours;
        glm::ivec2 m_cachedViewport;

        // Gizmos rendering object
        HG::Rendering::OpenGL::GizmosRenderer* m_gizmosRenderer;

        // ImGui rendering object
        HG::Rendering::OpenGL::ImGuiRenderer* m_imguiRenderer;

        // Blitting rendering object
        HG::Rendering::OpenGL::BlitRenderer* m_blitRenderer;

        // Map with renderbehaviour renderers.
        std::unordered_map<
            std::size_t,
            HG::Rendering::OpenGL::Forward::AbstractRenderer*
        > m_renderers;

        // Saved "current" render target
        HG::Rendering::Base::RenderTarget* m_savedRenderTarget;
    };
}

