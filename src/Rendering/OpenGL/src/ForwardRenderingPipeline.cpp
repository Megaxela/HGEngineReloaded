#include <ForwardRenderingPipeline.hpp>
#include <Application.hpp>
#include <GameObject.hpp>
#include <RenderBehaviour.hpp>
#include <Camera.hpp>
#include <Scene.hpp>

// Rendering behaviours
#include <Behaviours/Mesh.hpp>

OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application) :
    RenderingPipeline(application),
    m_meshFallback()
{
    Info() << "Initializing forward rendering.";

    Info() << "Creating fallback shader.";

    gl::shader vertexShader(GL_VERTEX_SHADER);
    gl::shader fragmentShader(GL_FRAGMENT_SHADER);

    vertexShader.set_source(
        "#version 330 core\n"
        "layout (location = 0) in vec3 Position;\n"
        "layout (location = 1) in vec2 TextureCoordinates;\n"
        "\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(Position, 1.0);\n"
        "    TexCoord = vec2(TextureCoordinates.x, TextureCoordinates.y);\n"
        "}"
    );

    fragmentShader.set_source(
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "\n"
        "// texture samplers\n"
        "uniform sampler2D texture1;\n"
        "uniform sampler2D texture2;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    // linearly interpolate between both textures (80% container, 20% awesomeface)\n"
        "    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
        "}"
    );

    m_meshFallback.attach_shader(vertexShader);
    m_meshFallback.attach_shader(fragmentShader);
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

void HG::Rendering::OpenGL::ForwardRenderingPipeline::renderMesh(::CORE_MODULE_NS::GameObject* gameObject,
                                                                 ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh* meshBehaviour)
{
    // todo: Add usage of material

    // Checking for VBO and VAO
    auto modelMatrix = gameObject->transform()->localToWorldMatrix();
    auto viewMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->viewMatrix();
    auto projectionMatrix = ::RENDERING_BASE_MODULE_NS::Camera::active()->projectionMatrix();


}
