#pragma once

// gl
#include <gl/all.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/BlitData.hpp>

// Forward declaration
namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class Material;
    class Texture;
    class RenderTarget;
}

namespace HG::Rendering::OpenGL
{
    /**
     * @brief Class, that describes object
     * for performing blitting.
     */
    class BlitRenderer
    {
    public:

        /**
         * @brief Constructor.
         * @param application Pointer to parent application.
         */
        explicit BlitRenderer(HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~BlitRenderer();

        /**
         * @brief Method for initialization blitter.
         */
        void init();

        /**
         * @brief Method for deinitialization blitter.
         */
        void deinit();

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method, that performs rendering data
         * to target rendertarget.
         * @param target Pointer to render target.
         * @param container Pointer to container.
         */
        void render(HG::Rendering::Base::RenderTarget *target,
                    HG::Rendering::Base::Texture *texture,
                    const HG::Rendering::Base::BlitData::DataContainer &container);

    private:

        HG::Core::Application* m_application;
        HG::Rendering::Base::Material* m_material;

        gl::buffer m_vbo;
        gl::buffer m_ebo;

        GLint m_uniformLocationTexture;
        GLint m_uniformLocationTextureSize;
        GLint m_uniformLocationProjection;

        GLuint m_attributeLocationVertices;
        GLuint m_attributeLocationUV;
    };
}


