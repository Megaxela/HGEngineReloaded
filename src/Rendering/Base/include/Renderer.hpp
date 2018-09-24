#pragma once

// HG::Utils
#include <DoubleBufferContainer.hpp>

namespace HG::Core
{
    class Application;
    class GameObject;
}

namespace HG::Rendering::Base
{
    class Camera;
    class RenderingPipeline;
    class MaterialCollection;
    class Gizmos;
    class RenderData;

    /**
     * @brief Class, that describes
     * layer between rendering pipeline and
     * user behaviour rendering system.
     */
    class Renderer
    {
    public:

        /**
         * @brief Default constructor.
         */
        explicit Renderer(HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~Renderer();

        /**
         * @brief Method for setting pipeline
         * object.
         * @tparam PipelineType Type of pipeline object.
         */
        void setPipeline(HG::Rendering::Base::RenderingPipeline* pipeline);

        /**
         * @brief Method for getting pipeline object.
         * @return Pointer to pipeline object.
         */
        HG::Rendering::Base::RenderingPipeline* pipeline();

        /**
         * @brief Method for initializing pipeline.
         */
        bool init();

        /**
         * @brief Method for deinitializing pipeline.
         */
        void deinit();

        /**
         * @brief Perform rendering finally.
         */
        void render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& gameObjects);

        /**
         * @brief Method for getting gizmos object.
         * @return Pointer to gizmos.
         */
        HG::Rendering::Base::Gizmos* gizmos();

        /**
         * @brief Method for getting renderer material collection.
         * @return Pointer to material collection.
         */
        HG::Rendering::Base::MaterialCollection* materialCollection();

        /**
         * @brief Method for getting active camera.
         * @return May return nullptr if there is no active camera.
         */
        HG::Rendering::Base::Camera* activeCamera() const;

        /**
         * @brief Method for setting active camera.
         * Camera has to be attached to gameObject.
         * Otherwise it will not be set.
         * @param camera Pointer to camera.
         */
        void setActiveCamera(HG::Rendering::Base::Camera* camera);

        /**
         * @brief Method for passing setup to
         * rendering pipeline. Also checks
         * availability of rendering pipeline.
         * @param data Pointer to render data.
         * @return Success.
         */
        bool setup(HG::Rendering::Base::RenderData* data);

    private:

        HG::Core::Application* m_parentApplication;

        HG::Rendering::Base::RenderingPipeline* m_pipeline;

        HG::Rendering::Base::Gizmos* m_gizmos;

        HG::Rendering::Base::MaterialCollection* m_materialCollection;

        HG::Rendering::Base::Camera* m_activeCamera;

    };
}

