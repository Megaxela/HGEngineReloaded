#pragma once

// HG::Rendering::Base
#include <RenderingPipeline.hpp> // Required for inheritance

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
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects) override;

        /**
         * @brief Method for rendering specified render
         * behaviour.
         * @param behaviour
         */
        bool render(HG::Rendering::Base::RenderBehaviour* behaviour) override;

        /**
         * @brief Init method.
         * @return Init success.
         */
        bool init() override;

        /**
         * @brief Deinit method.
         */
        void deinit() override;

    private:

        /**
         * @brief Method for processing game objects and it's behaviours.
         * @param objects Objects.
         */
        void proceedGameObjects(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects);

        // Caching
        std::vector<HG::Rendering::Base::RenderBehaviour*> m_behavioursCache;
        std::multimap<float, HG::Rendering::Base::RenderBehaviour*> m_sortedBehaviours;

        // Gizmos rendering object
        HG::Rendering::OpenGL::GizmosRenderer* m_gizmosRenderer;

        // ImGui rendering object
        HG::Rendering::OpenGL::ImGuiRenderer* m_imguiRenderer;

        // Map with renderbehaviour renderers.
        std::map<
            std::size_t,
            HG::Rendering::OpenGL::Forward::AbstractRenderer*
        > m_renderers;
    };
}

