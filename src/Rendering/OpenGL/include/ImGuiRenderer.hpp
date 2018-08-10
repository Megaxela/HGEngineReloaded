#pragma once

#include <gl/all.hpp>

// Forward declaration
namespace HG
{
    namespace Core
    {
        class Application;
    }

    namespace Rendering::Base
    {
        class Material;
    }
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
         * @brief Method for rendering prepared data.
         */
        void render();

    private:

        void createFontsTexture();

        HG::Core::Application* m_application;
        HG::Rendering::Base::Material* m_material;

        gl::buffer m_vbo;
        gl::buffer m_ebo;

        gl::texture_2d m_fontTexture;

        GLint m_uniformLocationTex = 0,
              m_uniformLocationProjMtx = 0;
        GLuint m_attribLocationPosition = 0,
               m_attribLocationUV = 0,
               m_attribLocationColor = 0;
    };
}


