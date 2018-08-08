#include <Materials/SpriteMaterial.hpp>
#include <Mesh.hpp>
#include <Common/ShaderData.hpp>
#include <Common/Texture2DData.hpp>
#include <Behaviours/Sprite.hpp>
#include <Camera.hpp>
#include "Forward/SpriteRenderer.hpp"
#include <gl/auxiliary/glm_uniforms.hpp>

OGL_RENDERING_MODULE_NS::Forward::SpriteRenderer::SpriteRenderer() :
    m_spriteMaterial(nullptr),
    m_spriteData(nullptr)
{

}

OGL_RENDERING_MODULE_NS::Forward::SpriteRenderer::~SpriteRenderer()
{
    delete m_spriteMaterial;
    delete m_spriteData;
}

void OGL_RENDERING_MODULE_NS::Forward::SpriteRenderer::init()
{
    Info() << "Creating sprite shader";

    m_spriteMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<Materials::SpriteMaterial>();

    // Initializing MeshData
    m_spriteData = new Common::MeshData();

    // Binding vertex array
    m_spriteData->VAO.bind();

    // Binding vertex buffer object
    m_spriteData->VBO.bind(GL_ARRAY_BUFFER);

    ::UTILS_MODULE_NS::Mesh mesh;

    float scale = 0.01f;

    // Image is flipped by y
    mesh.Vertices = {
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{ 0.5f * scale, -0.5f * scale,  0}, {0.0f,  1.0f}},
        {{ 0.5f * scale,  0.5f * scale,  0}, {0.0f,  0.0f}},
        {{-0.5f * scale, -0.5f * scale,  0}, {1.0f,  1.0f}},
        {{-0.5f * scale,  0.5f * scale,  0}, {1.0f,  0.0f}},
    };

    mesh.Indices = {
        0, 1, 2, 3, 4, 5
    };

    // Loading data into VBO
    m_spriteData->VBO.set_data(
        mesh.Vertices.size() * sizeof(::UTILS_MODULE_NS::Vertex),
        mesh.Vertices.data()
    );

    // Binding element buffer object
    m_spriteData->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    m_spriteData->EBO.set_data(
        mesh.Indices.size() * sizeof(uint32_t),
        mesh.Indices.data()
    );

    // Binding vertex buffer
    m_spriteData->VAO.set_vertex_buffer(0, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    m_spriteData->VAO.set_vertex_buffer(1, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    m_spriteData->VAO.set_vertex_buffer(2, m_spriteData->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));

    // Enabling attributes
    m_spriteData->VAO.set_attribute_enabled(0, true);
    m_spriteData->VAO.set_attribute_enabled(1, true);
    m_spriteData->VAO.set_attribute_enabled(2, true);
    m_spriteData->VAO.set_attribute_enabled(3, true);
    m_spriteData->VAO.set_attribute_enabled(4, true);

    // Setting
    m_spriteData->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, position)));
    m_spriteData->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, normal)));
    m_spriteData->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, uv)));
    m_spriteData->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, tangent)));
    m_spriteData->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, bitangent)));
}

void OGL_RENDERING_MODULE_NS::Forward::SpriteRenderer::render(CORE_MODULE_NS::GameObject *gameObject,
                                                              RENDERING_BASE_MODULE_NS::RenderBehaviour *renderBehaviour)
{
    auto spriteBehaviour = static_cast<RENDERING_BASE_MODULE_NS::Behaviours::Sprite*>(renderBehaviour);

    auto* program = m_spriteMaterial->shader()->externalData<Common::ShaderData>();
    auto* spriteExternal = spriteBehaviour->texture()->externalData<Common::Texture2DData>();

    if (!spriteExternal)
    {
        // todo: Add fallback sprite image
        Error() << "Sprite texture was not defined.";
        return;
    }

    program->Program.use();

    GLint location;

    if ((location = program->Program.uniform_location("model")) != -1)
    {
        program->Program.set_uniform(
            location,
            gameObject->transform()->localToWorldMatrix()
        );
    }

    if ((location = program->Program.uniform_location("view")) != -1)
    {
        program->Program.set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix()
        );
    }

    if ((location = program->Program.uniform_location("projection")) != -1)
    {
        program->Program.set_uniform(
            location,
            ::RENDERING_BASE_MODULE_NS::Camera::active()
                ->projectionMatrix()
        );
    }

    if ((location = program->Program.uniform_location("size")) != -1)
    {
        program->Program.set_uniform(
            location,
            glm::vec2(
                spriteBehaviour->texture()->surface()->Width,
                spriteBehaviour->texture()->surface()->Height
            )
        );
    }


    if ((location = program->Program.uniform_location("tex")) != -1)
    {
        program->Program.set_uniform_1i(location, 0);

        spriteExternal->Texture.set_active(0);
        spriteExternal->Texture.bind();
    }

    m_spriteData->VAO.bind();

    gl::set_blending_enabled(true);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(6),
        static_cast<GLsizei>(6),
        GL_UNSIGNED_INT
    );

    m_spriteData->VAO.unbind();
}

size_t OGL_RENDERING_MODULE_NS::Forward::SpriteRenderer::getTarget()
{
    return ::RENDERING_BASE_MODULE_NS::Behaviours::Sprite::Id;
}
