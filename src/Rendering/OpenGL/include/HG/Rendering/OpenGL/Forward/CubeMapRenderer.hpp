#pragma once

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Forward/AbstractRenderer.hpp> // Required for inheritance
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>

namespace HG::Rendering::Base
{
    class Material;
}

namespace HG::Rendering::OpenGL::Forward
{
    /**
     * @brief Class, that describes forward cubemap
     * renderer.
     */
    class CubeMapRenderer : public HG::Rendering::OpenGL::Forward::AbstractRenderer,
                            private HG::Rendering::OpenGL::Common::MaterialProcessor
    {
    public:

        /**
         * @brief Constructor.
         */
        CubeMapRenderer();

        /**
         * @brief Destructor.
         */
        ~CubeMapRenderer() override;

        /**
         * @brief Method for initializing
         * cubemap renderer.
         */
        void init() override;

        /**
         * @brief Method for deinitialization
         * cubemap renderer.
         */
        void deinit() override;

        /**
         * @brief Method that performs actual cubemap rendering.
         * @param renderBehaviour Pointer to render behaviour.
         */
        void render(HG::Rendering::Base::RenderBehaviour* renderBehaviour) override;

        /**
         * @brief Method for getting render behaviours type,
         * that this renderer can proceeed.
         * (HG::Rendering::Base::Behaviours::CubeMap)
         * @return
         */
        size_t getTarget() override;

    private:

        gl::vertex_array m_vao;
        gl::buffer m_vbo;

        HG::Rendering::Base::Material* m_skyboxMaterial;
    };

}
