
#include <GizmosRenderer.hpp>
#include <Camera.hpp>
#include <Material.hpp>
#include <ForwardRenderingPipeline.hpp>
#include <gl/auxiliary/glm_uniforms.hpp>

OGL_RENDERING_MODULE_NS::GizmosRenderer::GizmosRenderer(::CORE_MODULE_NS::Application* application) :
    m_application(application),
    m_lineShader(),
    m_meshShader(),
    m_linesVAO(gl::invalid_id),
    m_linesVBO(gl::invalid_id)
{

}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::init()
{
    m_linesVAO = std::move(gl::vertex_array());
    m_linesVBO = std::move(gl::buffer());

    // Preparing line shader
    m_lineShader.setShaderText(
        R"(
#ifdef VertexShader
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
out vec4 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    Color = color;
    gl_Position = projection * view * vec4(vertex, 1.0);
}
#endif

#ifdef FragmentShader
in vec4 Color;
out vec4 color;

void main()
{
    color = Color;
}
#endif
)"
    );

    m_application->renderer()->setup(&m_lineShader);

    // Preparing VAO
    m_linesVAO.bind();

    // Binding vertex buffer object
    m_linesVBO.bind(GL_ARRAY_BUFFER);

    // Binding vertex buffers
    m_linesVAO.set_vertex_buffer(0, m_linesVBO, 0, sizeof(RENDERING_BASE_MODULE_NS::Gizmos::LineData) / 2);
    m_linesVAO.set_vertex_buffer(1, m_linesVBO, 0, sizeof(RENDERING_BASE_MODULE_NS::Gizmos::LineData) / 2);

    // Enabling attributes
    m_linesVAO.set_attribute_enabled(0, true);
    m_linesVAO.set_attribute_enabled(1, true);

    // Setting up
    m_linesVAO.set_attribute_format(0, 3, GL_FLOAT, false, offsetof(RENDERING_BASE_MODULE_NS::Gizmos::LineData, begin));
    m_linesVAO.set_attribute_format(1, 4, GL_FLOAT, false, offsetof(RENDERING_BASE_MODULE_NS::Gizmos::LineData, beginColor));
}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::line(const ::RENDERING_BASE_MODULE_NS::Gizmos::LineData &line)
{
    m_lineData.push_back(line);
}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::sphere(const ::RENDERING_BASE_MODULE_NS::Gizmos::SphereData &sphere)
{

}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::hexahedron(const ::RENDERING_BASE_MODULE_NS::Gizmos::HexahedronData &hexahedron)
{

}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::render()
{
    renderLines();
}

void OGL_RENDERING_MODULE_NS::GizmosRenderer::renderLines()
{
    auto camera = RENDERING_BASE_MODULE_NS::Camera::active();

    // If there is no camera - skip rendering
    if (!camera)
    {
        m_lineData.clear();
        return;
    }

    m_linesVBO.set_data(
        sizeof(RENDERING_BASE_MODULE_NS::Gizmos::LineData) * m_lineData.size(),
        m_lineData.data()
    );

    auto* program = &m_lineShader.externalData<ForwardRenderingPipeline::ShaderData>()->Program;

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

    m_linesVAO.unbind();

    m_lineData.clear();
}