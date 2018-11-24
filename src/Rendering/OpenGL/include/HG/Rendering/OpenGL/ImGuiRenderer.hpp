#pragma once

// gl
#include <gl/all.hpp>

// Forward declaration
namespace HG::Core
{
    class Application;
}

namespace HG::Rendering::Base
{
    class Material;
    class Texture;
}

namespace HG::Rendering::OpenGL
{
    /**
     * @brief Class, that describes object for
     * ImGui rendering.
     */
    class ImGuiRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit ImGuiRenderer(HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~ImGuiRenderer();

        /**
         * @brief Method for initialization ImGui renderer.
         */
        void init();

        /**
         * @brief Method for deinitiazliation ImGui renderer.
         */
        void deinit();

        /**
         * @brief Method for rendering prepared data.
         */
        void render();

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

    private:

        void createFontsTexture();

        HG::Core::Application* m_application;
        HG::Rendering::Base::Material* m_material;

        gl::buffer m_vbo;
        gl::buffer m_ebo;

        HG::Rendering::Base::Texture* m_fontTexture;

        GLint m_uniformLocationTex = 0;
        GLint m_uniformLocationProjMtx = 0;

        GLuint m_attribLocationPosition = 0;
        GLuint m_attribLocationUV = 0;
        GLuint m_attribLocationColor = 0;
    };
}


