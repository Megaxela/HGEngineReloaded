#pragma once

#include <Gizmos.hpp>
#include <Shader.hpp>
#include <Application.hpp>
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL
{
    /**
     * @brief Class, for rendering gizmos system.
     */
    class GizmosRenderer
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit GizmosRenderer(::HG::Core::Application* application);

        /**
         * @brief Destructor.
         */
        ~GizmosRenderer();

        /**
         * @brief Method for initialization gizmos renderer.
         */
        void init();

        /**
         * @brief Method for preparing line for rendering.
         * @param line Line data.
         */
        void line(const ::HG::Rendering::Base::Gizmos::LineData& line);

        /**
         * @brief Method for preparing sphere for rendering.
         * @param sphere Sphere data.
         */
        void sphere(const ::HG::Rendering::Base::Gizmos::SphereData& sphere);

        /**
         * @brief Method for preparing hexahedron for rendering.
         * @param hexahedron Hexahedron data.
         */
        void hexahedron(const ::HG::Rendering::Base::Gizmos::HexahedronData& hexahedron);

        /**
         * @brief Method for rendering prepared data.
         */
        void render();

    private:

        void renderLines();

        ::HG::Core::Application* m_application;

        ::HG::Rendering::Base::Material* m_lineMaterial;
        ::HG::Rendering::Base::Material* m_meshMaterial;

        gl::vertex_array m_linesVAO;
        gl::buffer m_linesVBO;

        std::vector<::HG::Rendering::Base::Gizmos::LineData> m_lineData;
    };
}


