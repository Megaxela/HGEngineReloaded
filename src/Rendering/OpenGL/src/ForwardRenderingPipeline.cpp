#include <ForwardRenderingPipeline.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <RenderBehaviour.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>
#include <gl/auxiliary/glm_uniforms.hpp>

// Rendering behaviours
#include <Behaviours/Mesh.hpp>
#include <gl/all.hpp>
#include <imgui.h>

OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application) :
    RenderingPipeline(application),
    m_meshFallback(gl::invalid_id)
{

}

bool HG::Rendering::OpenGL::ForwardRenderingPipeline::init()
{
    if (!RenderingPipeline::init())
    {
        return false;
    }

    Info() << "Initializing forward rendering.";

    Info() << "Creating fallback shader.";

    gl::shader vertexShader(GL_VERTEX_SHADER);
    gl::shader fragmentShader(GL_FRAGMENT_SHADER);

    vertexShader.set_source(
        R"(
#version 420 core
layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    TexCoords = inTexCoords;
}
)"
    );

    if (!vertexShader.compile())
    {
        Error() << "Can't compile fallback vertex shader.";
        return false;
    }

    fragmentShader.set_source(
        R"(
#version 420 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D fallbackTexture;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)"
    );

    if (!fragmentShader.compile())
    {
        Error() << "Can't compile fallback fragment shader.";
        return false;
    }

    m_meshFallback = std::move(gl::program());

    if (!m_meshFallback.is_valid())
    {
        Error() << "Fallback shader is not valid.";
    }

    m_meshFallback.attach_shader(vertexShader);
    m_meshFallback.attach_shader(fragmentShader);
    if (!m_meshFallback.link())
    {
        Error() << "Can't link fallback shader.";
        return false;
    }

    return true;
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects)
{
    // Getting camera
    auto camera = ::RENDERING_BASE_MODULE_NS::Camera::active();

    if (camera == nullptr)
    {
        return;
    }

    // Clearing main buffer
    gl::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});

    gl::clear(
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
    );

    for (auto&& gameObject : objects)
    {
        m_behavioursCache.clear();

        // Getting rendering behaviours of GO
        gameObject->findRenderingBehaviours(m_behavioursCache);

        for (auto&& behaviour : m_behavioursCache)
        {
            switch (behaviour->behaviourType())
            {
            case ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Id:
                renderMesh(
                    gameObject,
                    static_cast<::RENDERING_BASE_MODULE_NS::Behaviours::Mesh*>(
                        behaviour
                    )
                );
                break;

            default:
                Info()
                    << "Trying to render unknown render behaviour \""
                    << SystemTools::getTypeName(behaviour)
                    << "\"";
                break;
            }
        }
    }

    ImGui::Render();

    application()->systemController()->swapBuffers();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(RENDERING_BASE_MODULE_NS::RenderBehaviour *behaviour)
{
    switch (behaviour->behaviourType())
    {
    case ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Id:
        setupMesh(static_cast<Base::Behaviours::Mesh *>(behaviour));
        break;
    default:
        Error() << "Setting up unknown render behaviour.";
        break;
    }
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setupMesh(::RENDERING_BASE_MODULE_NS::Behaviours::Mesh *behaviour)
{
    if (behaviour->mesh() == nullptr)
    {
        Error() << "Can't setup renderer without set mesh.";
        return;
    }

    if (behaviour->externalData<MeshData>() == nullptr)
    {
        behaviour->setExternalData<MeshData>();
    }

    auto data = behaviour->externalData<MeshData>();

    // Binding vertex array
    data->VAO.bind();

    // Binding vertex buffer object
    data->VBO.bind(GL_ARRAY_BUFFER);

    auto mesh = behaviour->mesh();

    // Loading data into VBO
    data->VBO.set_data(
        mesh->Vertices.size() * sizeof(::UTILS_MODULE_NS::Vertex),
        mesh->Vertices.data()
    );

    // Binding element buffer object
    data->EBO.bind(GL_ELEMENT_ARRAY_BUFFER);

    // Loading data into EBO
    data->EBO.set_data(
        mesh->Indices.size() * sizeof(uint32_t),
        mesh->Indices.data()
    );

    // Binding vertex buffer
    data->VAO.set_vertex_buffer(0, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    data->VAO.set_vertex_buffer(1, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));
    data->VAO.set_vertex_buffer(2, data->VBO, 0, sizeof(::UTILS_MODULE_NS::Vertex));

    // Enabling attributes
    data->VAO.set_attribute_enabled(0, true);
    data->VAO.set_attribute_enabled(1, true);
    data->VAO.set_attribute_enabled(2, true);
    data->VAO.set_attribute_enabled(3, true);
    data->VAO.set_attribute_enabled(4, true);

    // Setting
    data->VAO.set_attribute_format(0, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, position)));
    data->VAO.set_attribute_format(1, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, normal)));
    data->VAO.set_attribute_format(2, 2, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, uv)));
    data->VAO.set_attribute_format(3, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, tangent)));
    data->VAO.set_attribute_format(4, 3, GL_FLOAT, false, static_cast<GLuint>(offsetof(::UTILS_MODULE_NS::Vertex, bitangent)));

}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(::RENDERING_BASE_MODULE_NS::Texture* texture)
{
    // Checking surface on texture
    if (texture->surface() == nullptr)
    {
        Error() << "Can't setup texture without surface on it.";
        return;
    }

    // Creating external data if not presented
    if (texture->externalData<TextureData>() == nullptr)
    {
        texture->setExternalData<TextureData>();
    }

    auto externalData = texture->externalData<TextureData>();

    externalData->Texture.set_min_filter(GL_LINEAR);
    externalData->Texture.set_mag_filter(GL_LINEAR);

    GLuint fileFormat;

    // Getting type
    switch (texture->surface()->Bpp)
    {
    case 1:
        fileFormat = GL_RED;
        break;

    case 2:
        fileFormat = GL_RG;
        break;

    case 3:
        fileFormat = GL_RGB;
        break;

    case 4:
        fileFormat = GL_RGBA;
        break;

    default:
        Error() << "Can't setup texture because of unknown texture format.";
        break;
    }

    // Loading data into texture
    externalData->Texture.set_storage(
            1,       // Levels
            GL_RGBA8, // Internal format
            texture->surface()->Width,
            texture->surface()->Height
    );
    externalData->Texture.set_sub_image(
        0, // Level
        0, // X offset
        0, // Y Offset
        texture->surface()->Width,  // Width
        texture->surface()->Height, // Height
        fileFormat,       // Format
        GL_UNSIGNED_BYTE, // Type
        texture->surface()->Data
    );
    externalData->Texture.bind();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setup(::RENDERING_BASE_MODULE_NS::Shader *shader)
{
    // Creating external data if not presented
    if (shader->externalData<ShaderData>() == nullptr)
    {
        shader->setExternalData<ShaderData>();
    }

    auto externalData = shader->externalData<ShaderData>();

    gl::shader vertexShader(GL_VERTEX_SHADER);
    gl::shader fragmentShader(GL_FRAGMENT_SHADER);

    vertexShader.set_source(
        "#version 420 core\n"
        "#define VertexShader\n" +
        shader->shaderText()
    );

    if (!vertexShader.compile())
    {
        Error() << "Can't compile vertex shader. Error: " << vertexShader.info_log();
        return /* false */;
    }

    fragmentShader.set_source(
        "#version 420 core\n"
        "#define FragmentShader\n" +
        shader->shaderText()
    );

    if (!fragmentShader.compile())
    {
        Error() << "Can't compile fragment shader. Error: " << fragmentShader.info_log();
        return /* false */;
    }

    if (!externalData->Program.is_valid())
    {
        Error() << "Fallback shader is not valid.";
    }

    externalData->Program.attach_shader(vertexShader);
    externalData->Program.attach_shader(fragmentShader);
    if (!externalData->Program.link())
    {
        Error() << "Can't link shader. Error: " << externalData->Program.info_log();
        return /* false */;
    }
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::renderMesh(::CORE_MODULE_NS::GameObject* gameObject,
                                                                   ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* meshBehaviour)
{

    auto data = meshBehaviour->externalData<MeshData>();

    // todo: On errors, render "error" mesh instead.
    // Additional information in mesh is
    // contains VAO, VBO, EBO, so here is just
    // rendering of that info.
    if ( data == nullptr ||
        !data->VAO.is_valid() ||
        !data->VBO.is_valid() ||
        !data->EBO.is_valid())
    {
        Error()
            << "Mesh rendering behaviour of \""
            << meshBehaviour->gameObject()->name()
            << "\" has not external data or has non valid buffers.";
        return;
    }

    gl::program* program = nullptr;

    if (meshBehaviour->material() == nullptr ||
        meshBehaviour->material()->shader() == nullptr ||
        meshBehaviour->material()->shader()->externalData<ShaderData>() == nullptr)
    {
//        Error()
//            << "Mesh rendering behaviour of \""
//            << meshBehaviour->gameObject()->name()
//            << "\" has no material or material is wrong.";

        program = &m_meshFallback;

        program->use();
    }
    else
    {
        program = &meshBehaviour->material()->shader()->externalData<ShaderData>()->Program;

        program->use();

        m_textureNumber = 0;
        for (auto&& [uniformName, uniformValue] : *meshBehaviour->material())
        {
            setShaderUniform(program, uniformName, uniformValue);
        }
    }

    // Checking for VBO, VAO and EBO
    auto modelMatrix = gameObject->transform()->localToWorldMatrix();
    auto viewMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix();
    auto projectionMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->projectionMatrix();

    program->set_uniform(
        program->uniform_location("model"),
        modelMatrix
    );

    program->set_uniform(
        program->uniform_location("view"),
        viewMatrix
    );

    program->set_uniform(
        program->uniform_location("projection"),
        projectionMatrix
    );

    data->VAO.bind();

    gl::draw_range_elements(
        GL_TRIANGLES, // mode
        0,            // start
        static_cast<GLuint>(meshBehaviour->mesh()->Indices.size()),
        static_cast<GLsizei>(meshBehaviour->mesh()->Indices.size()),
        GL_UNSIGNED_INT
    );

    data->VAO.unbind();
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::setShaderUniform(gl::program *program,
                                                                         const std::string &name,
                                                                         const ::RENDERING_BASE_MODULE_NS::Material::Value &value)
{

    auto location = program->uniform_location(name);

    switch (value.type)
    {
    case Base::Material::Value::Type::Int:
        program->set_uniform(
            location,
            value.integer
        );
        break;
    case Base::Material::Value::Type::Float:
        program->set_uniform(
            location,
            value.floating
        );
        break;
    case Base::Material::Value::Type::Boolean:
        program->set_uniform(
            location,
            value.boolean
        );
        break;
    case Base::Material::Value::Type::Vector2:
        program->set_uniform(
            location,
            value.vector2
        );
        break;
    case Base::Material::Value::Type::Vector3:
        program->set_uniform(
            location,
            value.vector3
        );
        break;
    case Base::Material::Value::Type::Vector4:
        program->set_uniform(
            location,
            value.vector4
        );
        break;
    case Base::Material::Value::Type::Matrix2x2:
        program->set_uniform(
            location,
            value.mat2x2
        );
        break;
    case Base::Material::Value::Type::Matrix3x3:
        program->set_uniform(
            location,
            value.mat3x3
        );
        break;
    case Base::Material::Value::Type::Matrix4x4:
        program->set_uniform(
            location,
            value.mat4x4
        );
        break;
    case Base::Material::Value::Type::Texture:
        // todo: If any errors on texture, render fallback texture.
        // Setting texture unit
        program->set_uniform_1i(
            location,
            m_textureNumber
        );

        glActiveTexture(GL_TEXTURE0 + m_textureNumber);
        value.texture->externalData<TextureData>()->Texture.bind();

        ++m_textureNumber;

        break;
    }
}
