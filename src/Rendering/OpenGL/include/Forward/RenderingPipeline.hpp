#pragma once

#include <RenderingPipeline.hpp> // Inheritance

#include <gl/all.hpp>
#include <ImGuiRenderer.hpp>
#include <GizmosRenderer.hpp>

#include <Texture.hpp> // Required for enum

namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class Shader;
//    class Texture; // Already included
    class RenderBehaviour;

    namespace Behaviours
    {
        class Mesh;
    }
}

namespace HG::Rendering::OpenGL::Forward
{
    class AbstractRenderer;
    class AbstractRenderDataProcessor;

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
        RenderingPipeline* addRenderer(AbstractRenderer* renderer);

        /**
         * @brief Actual render method.
         * @param objects Container with objects.
         */
        void render(const HG::Core::Scene::GameObjectsContainer& objects) override;

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
        void proceedGameObjects(const HG::Core::Scene::GameObjectsContainer& objects);

        std::vector<HG::Rendering::Base::RenderBehaviour*> m_behavioursCache;

        std::multimap<float, HG::Rendering::Base::RenderBehaviour*> m_sortedBehaviours;

        // Gizmos rendering object
        GizmosRenderer m_gizmosRenderer;

        // ImGui rendering object
        ImGuiRenderer m_imguiRenderer;

        std::map<
            std::size_t,
            AbstractRenderer*
        > m_renderers;
    };
}

