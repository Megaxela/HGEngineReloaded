// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/CountStatistics.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Materials/GizmosLineMaterial.hpp>
#include <HG/Rendering/OpenGL/Materials/GizmosMeshMaterial.hpp>
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/GizmosRenderer.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Camera.hpp>

// gl
#include <gl/auxiliary/glm_uniforms.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Rendering::OpenGL::GizmosRenderer::GizmosRenderer(HG::Core::Application* application) :
    m_application(application),
    m_lineMaterial(nullptr),
    m_meshMaterial(nullptr),
    m_linesVAO(gl::invalid_id),
    m_linesVBO(gl::invalid_id)
{

}

HG::Rendering::OpenGL::GizmosRenderer::~GizmosRenderer()
{
    deinit();
}

void HG::Rendering::OpenGL::GizmosRenderer::deinit()
{
    Info() << "Deinitializing gizmos renderer";

    delete m_lineMaterial;
    delete m_meshMaterial;

    m_lineMaterial = nullptr;
    m_meshMaterial = nullptr;

    m_linesVAO = std::move(gl::vertex_array(gl::invalid_id));
    m_linesVBO = std::move(gl::buffer(gl::invalid_id));
}

void HG::Rendering::OpenGL::GizmosRenderer::init()
{
    Info() << "Initializing gizmos renderer";

    m_linesVAO = std::move(gl::vertex_array());
    m_linesVBO = std::move(gl::buffer());

    // Preparing line shader
    m_lineMaterial = m_application->renderer()->materialCollection()->getMaterial<Materials::GizmosLineMaterial>();
//    m_meshMaterial = m_application->renderer()->materialCollection()->getMaterial<Materials::GizmosMeshMaterial>();

    // Preparing VAO
    m_linesVAO.bind();

    // Binding vertex buffer object
    m_linesVBO.bind(GL_ARRAY_BUFFER);

    // Binding vertex buffers
    m_linesVAO.set_vertex_buffer(0, m_linesVBO, 0, sizeof(HG::Rendering::Base::Gizmos::LineData) / 2);
    m_linesVAO.set_vertex_buffer(1, m_linesVBO, 0, sizeof(HG::Rendering::Base::Gizmos::LineData) / 2);

    // Enabling attributes
    m_linesVAO.set_attribute_enabled(0, true);
    m_linesVAO.set_attribute_enabled(1, true);

    // Setting up
    m_linesVAO.set_attribute_format(0, 3, GL_FLOAT, false, offsetof(HG::Rendering::Base::Gizmos::LineData, begin));
    m_linesVAO.set_attribute_format(1, 4, GL_FLOAT, false, offsetof(HG::Rendering::Base::Gizmos::LineData, beginColor));
}

void HG::Rendering::OpenGL::GizmosRenderer::line(const HG::Rendering::Base::Gizmos::LineData &line)
{
    m_lineData.push_back(line);
}

void HG::Rendering::OpenGL::GizmosRenderer::sphere(const HG::Rendering::Base::Gizmos::SphereData &sphere)
{
    (void) sphere;
    // todo: Add implementation
}

void HG::Rendering::OpenGL::GizmosRenderer::hexahedron(const HG::Rendering::Base::Gizmos::HexahedronData &hexahedron)
{
    (void) hexahedron;
    // todo: Add implementation
}

void HG::Rendering::OpenGL::GizmosRenderer::render()
{
    renderLines();
}

void HG::Rendering::OpenGL::GizmosRenderer::renderLines()
{
    auto camera = m_application->renderer()->activeCamera();

    // If there is no camera - skip rendering
    if (!camera)
    {
        m_lineData.clear();
        return;
    }

    m_linesVBO.set_data(
        sizeof(HG::Rendering::Base::Gizmos::LineData) * m_lineData.size(),
        m_lineData.data()
    );

    auto* program = &static_cast<Common::ShaderData*>(m_lineMaterial->shader()->specificData())->Program;

    program->set_uniform(
        program->uniform_location("projection"),
        camera->projectionMatrix()
    );

    program->set_uniform(
        program->uniform_location("view"),
        camera->viewMatrix()
    );

    program->use();

    m_linesVAO.bind();

    gl::draw_arrays(
        GL_LINES,
        0,
        m_lineData.size() * 2
    );

    if (m_application->countStatistics()->hasCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices))
    {
        m_application->countStatistics()->add(
            HG::Core::CountStatistics::CommonCounter::NumberOfVertices,
            m_lineData.size() * 2
        );
    }

    m_linesVAO.unbind();

    m_lineData.clear();
}
