#pragma once

#include <Gizmos.hpp>
#include <Shader.hpp>
#include <Application.hpp>
#include <gl/all.hpp>

namespace OGL_RENDERING_MODULE_NS
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
        explicit GizmosRenderer(::CORE_MODULE_NS::Application* application);

        /**
         * @brief Method for initialization gizmos renderer.
         */
        void init();

        /**
         * @brief Method for preparing line for rendering.
         * @param line Line data.
         */
        void line(const ::RENDERING_BASE_MODULE_NS::Gizmos::LineData& line);

        /**
         * @brief Method for preparing sphere for rendering.
         * @param sphere Sphere data.
         */
        void sphere(const ::RENDERING_BASE_MODULE_NS::Gizmos::SphereData& sphere);

        /**
         * @brief Method for preparing hexahedron for rendering.
         * @param hexahedron Hexahedron data.
         */
        void hexahedron(const ::RENDERING_BASE_MODULE_NS::Gizmos::HexahedronData& hexahedron);

        /**
         * @brief Method for rendering prepared data.
         */
        void render();

    private:

        void renderLines();

        ::CORE_MODULE_NS::Application* m_application;

        ::RENDERING_BASE_MODULE_NS::Shader m_lineShader;
        ::RENDERING_BASE_MODULE_NS::Shader m_meshShader;

        gl::vertex_array m_linesVAO;
        gl::buffer m_linesVBO;

        std::vector<::RENDERING_BASE_MODULE_NS::Gizmos::LineData> m_lineData;
    };
}


