#pragma once

// HG::Utils
#include <HG/Utils/DoubleBufferContainer.hpp>
#include <HG/Utils/Color.hpp>

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
    class RenderTarget;
    class CubeMap;
    class Texture;

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
         * @brief Method for getting default render target.
         * @return Pointer to default render target.
         */
        HG::Rendering::Base::RenderTarget* defaultRenderTarget() const;

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

        /**
         * @brief Method for checking is render data requires
         * setup?
         * @param data Pointer to data.
         */
        bool needSetup(HG::Rendering::Base::RenderData* data);

        /**
         * @brief Method for getting currently active cube map
         * object. Can be nullptr.
         * @return Pointer to currently active cube map.
         */
        HG::Rendering::Base::CubeMap* activeCubeMap() const;

        /**
         * @brief Method for setting currently active cube map.
         * This method is called by CubeMapRenderer of
         * selected rendering pipeline. (Because cubemap is renders
         * at first).
         * @param cubemap Pointer to new active cube map.
         */
        void setActiveCubeMap(HG::Rendering::Base::CubeMap* cubemap);

        /**
         * @brief Method for getting texture pixel value. May
         * throw `std::runtime_error` if there is no pipeline set.
         * @param texture Pointer to texture.
         * @param pos Pixel position.
         * @return Pixel color.
         */
        HG::Utils::Color getTexturePixel(HG::Rendering::Base::Texture* texture, glm::ivec2 pos);

    private:

        HG::Core::Application* m_parentApplication;

        HG::Rendering::Base::RenderingPipeline* m_pipeline;

        HG::Rendering::Base::Gizmos* m_gizmos;

        HG::Rendering::Base::MaterialCollection* m_materialCollection;

        HG::Rendering::Base::Camera* m_activeCamera;

        HG::Rendering::Base::RenderTarget* m_defaultRenderTarget;

        HG::Rendering::Base::CubeMap* m_activeCubemap;
    };
}

