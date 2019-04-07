#pragma once

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MaterialProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Gizmos.hpp>

// HG::Utils
#include <HG/Utils/Interfaces/Initializable.hpp>

// gl
#include <gl/all.hpp>

namespace HG::Rendering::Base
{
    class Material;
}

namespace HG::Rendering::OpenGL
{
    /**
     * @brief Class, for rendering gizmos system.
     */
    class GizmosRenderer : public HG::Utils::Interfaces::Initializable,
                           private HG::Rendering::OpenGL::Common::MaterialProcessor
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit GizmosRenderer(HG::Core::Application* application);

        /**
         * @brief Method for preparing line for rendering.
         * @param line Line data.
         */
        void line(const HG::Rendering::Base::Gizmos::LineData& line);

        /**
         * @brief Method for preparing sphere for rendering.
         * @param sphere Sphere data.
         */
        void sphere(const HG::Rendering::Base::Gizmos::SphereData& sphere);

        /**
         * @brief Method for preparing hexahedron for rendering.
         * @param hexahedron Hexahedron data.
         */
        void hexahedron(const HG::Rendering::Base::Gizmos::HexahedronData& hexahedron);

        /**
         * @brief Method for rendering prepared data.
         */
        void render();

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        HG::Core::Application* application() const;

    protected:

        /**
         * @brief Method for initialization gizmos renderer.
         */
        void onInit() override;

        /**
         * @brief Method for deinitialization gizmos renderer.
         */
        void onDeinit() override;

    private:

        void renderLines();

        HG::Core::Application* m_application;

        HG::Rendering::Base::Material* m_lineMaterial;
        HG::Rendering::Base::Material* m_meshMaterial;

        gl::vertex_array m_linesVAO;
        gl::buffer m_linesVBO;

        std::vector<HG::Rendering::Base::Gizmos::LineData> m_lineData;
    };
}


