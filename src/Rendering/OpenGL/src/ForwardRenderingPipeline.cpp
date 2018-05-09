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
        "#version 420 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main()\n"
        "{\n"
        "\tgl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
        "\tTexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "}"
    );

    if (!vertexShader.compile())
    {
        Error() << "Can't compile fallback vertex shader.";
        return false;
    }

    fragmentShader.set_source(
        "#version 420 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "\n"
        "// texture samplers\n"
        "uniform sampler2D texture1;\n"
        "\n"
        "void main()\n"
        "{\n"
        "\t// linearly interpolate between both textures (80% container, 20% awesomeface)\n"
        "\t//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
        "\tFragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}"
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
        return true;
    }
}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::render(const ::CORE_MODULE_NS::Scene::GameObjectsContainer& objects)
{
    // Getting camera
    auto camera = ::RENDERING_BASE_MODULE_NS::Camera::active();

    if (camera == nullptr)
    {
        return;
    }

    m_behavoursCache.clear();

    for (auto&& gameObject : objects)
    {
        // Getting rendering behaviours of GO
        gameObject->findRenderingBehaviours(m_behavoursCache);

        for (auto&& behaviour : m_behavoursCache)
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

    // Position
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(::UTILS_MODULE_NS::Vertex),
        nullptr
    );
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(::UTILS_MODULE_NS::Vertex),
        (GLvoid*) (3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(::UTILS_MODULE_NS::Vertex),
        (GLvoid*) (6 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(::UTILS_MODULE_NS::Vertex),
        (GLvoid*) (8 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(3);

    // Bitangent
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(::UTILS_MODULE_NS::Vertex),
        (GLvoid*) (11 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(4);

}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::renderMesh(::CORE_MODULE_NS::GameObject* gameObject,
                                                                   ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* meshBehaviour)
{
    // todo: On errors, render "error" mesh instead.
    // Additional information in mesh is
    // contains VAO, VBO, EBO, so here is just
    // rendering of that info.
    if (meshBehaviour->externalData<MeshData>() == nullptr)
    {
        Error()
            << "Mesh rendering behaviour of \""
            << meshBehaviour->gameObject()->name()
            << "\" has not external data.";
        return;
    }

    auto data = meshBehaviour->externalData<MeshData>();

    if (!data->VAO.is_valid() ||
        !data->VBO.is_valid() ||
        !data->EBO.is_valid())
    {
        Error()
            << "External data of \""
            << meshBehaviour->gameObject()->name()
            << "\" mesh behaviour has non valid buffers.";
        return;
    }

    // todo: Add usage of material

    // Checking for VBO and VAO
    auto modelMatrix = gameObject->transform()->localToWorldMatrix();
    auto viewMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix();
    auto projectionMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->projectionMatrix();

    m_meshFallback.use();

    m_meshFallback.set_uniform(
        m_meshFallback.uniform_location("model"),
        modelMatrix
    );

    m_meshFallback.set_uniform(
        m_meshFallback.uniform_location("view"),
        viewMatrix
    );

    m_meshFallback.set_uniform(
        m_meshFallback.uniform_location("projection"),
        projectionMatrix
    );

    data->VAO.bind();
    gl::draw_elements(
        GL_TRIANGLES,
        (GLsizei) meshBehaviour->mesh()->Indices.size(),
        GL_UNSIGNED_INT,
        nullptr
    );
    data->VAO.unbind();
}
