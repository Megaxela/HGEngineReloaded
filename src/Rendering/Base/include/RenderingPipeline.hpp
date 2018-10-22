#pragma once

// C++ STL
#include <unordered_map>

// HG::Utils
#include <DoubleBufferContainer.hpp>
#include <Color.hpp>

namespace HG::Core
{
    class Application;
    class GameObject;
}

namespace HG::Rendering::Base
{
    class RenderBehaviour;
    class AbstractRenderDataProcessor;
    class RenderData;
    class RenderTarget;

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
        explicit RenderingPipeline(HG::Core::Application* application);

        /**
         * @brief Desturctor.
         */
        virtual ~RenderingPipeline();

        /**
         * @brief Method for initializing pipeline.
         * By default it just initialize system controller
         * and creates window.
         */
        virtual bool init();

        /**
         * @brief Method for deinitialization pipeline.
         * By default it just deinitialize system controller
         * and creates window.
         */
        virtual void deinit();

        /**
         * @brief Method for clearing current
         * render target.
         * @param color Clear color.
         */
        virtual void clear(HG::Utils::Color color) = 0;

        /**
         * @brief Rendering method.
         * @param objects Objects.
         */
        virtual void render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects) = 0;

        /**
         * @brief Method for rendering specified
         * render behaviour.
         * @param behaviour Pointer to render behaviour.
         * @return Render sucсeed.
         */
        virtual bool render(HG::Rendering::Base::RenderBehaviour* behaviour) = 0;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method for setting up render behaviour.
         * @param data Pointer to render behaviour.
         */
        virtual bool setup(HG::Rendering::Base::RenderData* data);

        /**
         * @brief Method for checking is render data needs
         * to set up.
         * @param data Pointer to render data.
         * @return Is render data required to be set up.
         */
        virtual bool needSetup(HG::Rendering::Base::RenderData* data);

        /**
         * @brief Method for adding render data processor.
         * That will setup any render data.
         * @param processor Pointer to processor.
         */
        HG::Rendering::Base::RenderingPipeline* addRenderDataProcessor(HG::Rendering::Base::AbstractRenderDataProcessor* processor);

        /**
         * @brief Method for settring render target.
         * @param target Pointer to render target.
         */
        virtual void setRenderTarget(HG::Rendering::Base::RenderTarget* target);

        /**
         * @brief Method for getting render target.
         * @return Pointer to render target.
         * (Default render target by default)
         */
        HG::Rendering::Base::RenderTarget* renderTarget() const;

    protected:

        /**
         * @brief Method, that's called, when render behaviour has to be
         * set up.
         * @param behaviour Pointer to behaviour.
         */
        virtual bool setupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour);

        /**
         * @brief Method, that's called, when render behaviour has to be
         * checked for set up.
         * @param behaviour Pointer to behaviour.
         */
        virtual bool needSetupRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour);

    private:

        HG::Core::Application* m_parentApplication;
        HG::Rendering::Base::RenderTarget* m_currentRenderTarget;

        std::unordered_map<
            std::size_t,
            HG::Rendering::Base::AbstractRenderDataProcessor*
        > m_renderDataProcessor;
    };
}

