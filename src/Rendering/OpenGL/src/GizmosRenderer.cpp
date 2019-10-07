// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/CountStatistics.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>
#include <HG/Rendering/OpenGL/GizmosRenderer.hpp>
#include <HG/Rendering/OpenGL/Materials/GizmosLineMaterial.hpp>
#include <HG/Rendering/OpenGL/Materials/GizmosMeshMaterial.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>

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

void HG::Rendering::OpenGL::GizmosRenderer::onDeinit()
{
    Info() << "Deinitializing gizmos renderer";

    delete m_lineMaterial;
    delete m_meshMaterial;

    m_lineMaterial = nullptr;
    m_meshMaterial = nullptr;

    m_linesVAO = std::move(gl::vertex_array(gl::invalid_id));
    m_linesVBO = std::move(gl::buffer(gl::invalid_id));
}

void HG::Rendering::OpenGL::GizmosRenderer::onInit()
{
    Info() << "Initializing gizmos renderer";

    m_linesVAO = std::move(gl::vertex_array());
    m_linesVBO = std::move(gl::buffer());

    // Preparing line shader
    m_lineMaterial = application()->renderer()->materialCollection()->getMaterial<Materials::GizmosLineMaterial>();
    //    m_meshMaterial = application()->renderer()->materialCollection()->getMaterial<Materials::GizmosMeshMaterial>();

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

HG::Core::Application* HG::Rendering::OpenGL::GizmosRenderer::application() const
{
    return m_application;
}

void HG::Rendering::OpenGL::GizmosRenderer::line(const HG::Rendering::Base::Gizmos::LineData& line)
{
    m_lineData.push_back(line);
}

void HG::Rendering::OpenGL::GizmosRenderer::sphere(const HG::Rendering::Base::Gizmos::SphereData& sphere)
{
    (void)sphere;
    // todo: Add implementation
}

void HG::Rendering::OpenGL::GizmosRenderer::hexahedron(const HG::Rendering::Base::Gizmos::HexahedronData& hexahedron)
{
    (void)hexahedron;
    // todo: Add implementation
}

void HG::Rendering::OpenGL::GizmosRenderer::render()
{
    renderLines();
}

void HG::Rendering::OpenGL::GizmosRenderer::renderLines()
{
    BENCH("Rendering gizmos lines");
    auto camera = application()->renderer()->activeCamera();

    // If there is no camera - skip rendering
    if (!camera)
    {
        m_lineData.clear();
        return;
    }

    m_linesVBO.set_data(sizeof(HG::Rendering::Base::Gizmos::LineData) * m_lineData.size(), m_lineData.data());

    m_lineMaterial->set("projection", camera->projectionMatrix());
    m_lineMaterial->set("view", camera->viewMatrix());
    applyMaterialUniforms(application(), m_lineMaterial);
    useMaterial(application(), m_lineMaterial);

    m_linesVAO.bind();

    gl::draw_arrays(GL_LINES, 0, m_lineData.size() * 2);

    if (application()->countStatistics()->hasCounter(HG::Core::CountStatistics::CommonCounter::NumberOfVertices))
    {
        application()->countStatistics()->add(HG::Core::CountStatistics::CommonCounter::NumberOfVertices,
                                              m_lineData.size() * 2);
    }

    m_linesVAO.unbind();

    m_lineData.clear();
}
