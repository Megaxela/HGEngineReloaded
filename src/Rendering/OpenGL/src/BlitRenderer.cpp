// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/BlitRenderer.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderData.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DData.hpp>
#include <HG/Rendering/OpenGL/Materials/BlitMaterial.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// gl
#include <gl/auxiliary/glm_uniforms.hpp>

// glm
#include <glm/gtc/matrix_transform.hpp>

namespace HG::Rendering::OpenGL
{
BlitRenderer::BlitRenderer(HG::Core::Application* application) :
    m_application(application),
    m_material(nullptr),
    m_vbo(gl::invalid_id),
    m_ebo(gl::invalid_id),
    m_uniformLocationTexture(0),
    m_uniformLocationTextureSize(0),
    m_uniformLocationProjection(0),
    m_attributeLocationVertices(0),
    m_attributeLocationUV(0)
{
}

void BlitRenderer::onInit()
{
    HGInfo() << "Initializing blitter";

    m_material = application()->renderer()->materialCollection()->getMaterial<Materials::BlitMaterial>();

    m_vbo = std::move(gl::buffer());
    m_ebo = std::move(gl::buffer());

    auto* program = &m_material->shader()->castSpecificDataTo<Common::ShaderData>()->Program;

    m_uniformLocationTexture     = program->uniform_location("sourceTexture");
    m_uniformLocationTextureSize = program->uniform_location("textureSize");
    m_uniformLocationProjection  = program->uniform_location("projection");

    m_attributeLocationVertices = program->attribute_location("vertex");
    m_attributeLocationUV       = program->attribute_location("uvPixels");
}

void BlitRenderer::onDeinit()
{
    HGInfo() << "Deinitializing blitter";

    delete m_material;
    m_material = nullptr;

    m_vbo = std::move(gl::buffer(gl::invalid_id));
    m_ebo = std::move(gl::buffer(gl::invalid_id));
}

HG::Core::Application* BlitRenderer::application() const
{
    return m_application;
}

void BlitRenderer::render(HG::Rendering::Base::RenderTarget* target,
                          HG::Rendering::Base::Texture* texture,
                          const HG::Rendering::Base::BlitData::DataContainer& container)
{
    gl::set_polygon_face_culling_enabled(false);

    // Creating projection matrix
    const auto projection =
        glm::ortho(0.0f, static_cast<float>(target->size().x), static_cast<float>(target->size().y), 0.0f);

    m_material->set("projection", projection);
    m_material->set("textureSize", texture->size());
    m_material->set("sourceTexture", texture);
    applyMaterialUniforms(application(), m_material, true);
    useMaterial(application(), m_material);

    // Creating and setting up VAO
    gl::buffer vbo;
    gl::buffer ebo;

    gl::vertex_array vao;

    vao.bind();
    vbo.bind(GL_ARRAY_BUFFER);

    // Setting up VEO attributes
    vao.set_attribute_enabled(m_attributeLocationVertices, true);
    vao.set_attribute_enabled(m_attributeLocationUV, true);

    vao.set_vertex_buffer(m_attributeLocationVertices, vbo, 0, sizeof(HG::Rendering::Base::BlitData::PointData));
    vao.set_vertex_buffer(m_attributeLocationUV, vbo, 0, sizeof(HG::Rendering::Base::BlitData::PointData));

    vao.set_attribute_format(m_attributeLocationVertices,
                             2,
                             GL_INT,
                             false,
                             static_cast<GLuint>(offsetof(HG::Rendering::Base::BlitData::PointData, point)));
    vao.set_attribute_format(m_attributeLocationUV,
                             2,
                             GL_UNSIGNED_INT,
                             false,
                             static_cast<GLuint>(offsetof(HG::Rendering::Base::BlitData::PointData, uvPixels)));

    vbo.bind(GL_ARRAY_BUFFER);

    // Loading data into VBO
    vbo.set_data(container.vertices.size() * sizeof(HG::Rendering::Base::BlitData::PointData),
                 container.vertices.data(),
                 GL_STREAM_DRAW);

    // Binding EBO
    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    ebo.set_data(container.indices.size() * sizeof(std::uint32_t), container.indices.data(), GL_STREAM_DRAW);

    // Actually drawing
    gl::draw_elements(GL_TRIANGLES, static_cast<GLsizei>(container.indices.size()), GL_UNSIGNED_INT, nullptr);

    gl::set_polygon_face_culling_enabled(true);
}
} // namespace HG::Rendering::OpenGL
