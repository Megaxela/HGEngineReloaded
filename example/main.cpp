#include <Application.hpp>
#include <Scene.hpp>
#include <iostream>
#include <thread>
#include <GameObjectBuilder.hpp>
#include <Camera.hpp>
#include <Loggers/BasicLogger.hpp>
#include <Mesh.hpp>
#include <Behaviours/Mesh.hpp>
#include <Behaviours/FPSCameraMovement.hpp>
#include <imgui.h>
#include <Behaviours/ServiceInformationOverlay.hpp>
#include <FilesystemResourceAccessor.hpp>
#include <Loaders/STBImageLoader.hpp>
#include <Lights/PointLight.hpp>
#include "Behaviour.hpp"
#include "ForwardRenderingPipeline.hpp"
#include "GLFWSystemController.hpp"

class RotationBehaviour : public HG::Core::Behaviour
{
protected:

    void onUpdate() override
    {
        auto dt = scene()
            ->application()
            ->timeStatistics()
            ->lastFrameDeltaTime().count() / 1000000.0;

        gameObject()
            ->transform()
            ->setLocalRotation(
                gameObject()
                    ->transform()
                    ->localRotation() *
                glm::quat(glm::vec3(0, glm::radians(90.0f) * dt, 0))
        );
    }
};

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    InfoF() << "Creating application";
    HG::Core::Application application(argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()
            ->setResourceAccessor<HG::Standard::FilesystemResourceAccessor>();

    // Setting system controller
    application.setSystemController<HG::Rendering::OpenGL::GLFWSystemController>();

    // Setting rendering to forward
    application.renderer()
        ->setPipeline<HG::Rendering::OpenGL::ForwardRenderingPipeline>();

    if (!application.init())
    {
        ErrorF() << "Can't init application.";
        return -1;
    }

    auto* scene = new HG::Core::Scene();

    // Initial mesh
    auto mesh = std::make_shared<HG::Utils::Mesh>();

    mesh->Vertices = {
//        // Back face
        {{-0.5f, -0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {1.0f, 0.0f}}, // bottom-right
        {{ 0.5f,  0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {1.0f, 1.0f}}, // top-right
        {{ 0.5f,  0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {1.0f, 1.0f}}, // top-right
        {{-0.5f,  0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {0.0f, 1.0f}}, // top-left
        {{-0.5f, -0.5f, -0.5f}, { 0.0,  0.0,  1.0},  {0.0f, 0.0f}}, // bottom-left
        // Front face
        {{-0.5f, -0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {0.0f, 0.0f}}, // bottom-left
        {{ 0.5f,  0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {1.0f, 1.0f}}, // top-right
        {{ 0.5f, -0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {1.0f, 0.0f}}, // bottom-right
        {{ 0.5f,  0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {1.0f, 1.0f}}, // top-right
        {{-0.5f, -0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {0.0f, 0.0f}}, // bottom-left
        {{-0.5f,  0.5f,  0.5f}, { 0.0,  0.0, -1.0}, {0.0f, 1.0f}}, // top-left
        // Left face
        {{-0.5f,  0.5f,  0.5f}, {-1.0,  0.0,  0.0}, {1.0f, 0.0f}}, // top-right
        {{-0.5f, -0.5f, -0.5f}, {-1.0,  0.0,  0.0}, {0.0f, 1.0f}}, // bottom-left
        {{-0.5f,  0.5f, -0.5f}, {-1.0,  0.0,  0.0}, {1.0f, 1.0f}}, // top-left
        {{-0.5f, -0.5f, -0.5f}, {-1.0,  0.0,  0.0}, {0.0f, 1.0f}}, // bottom-left
        {{-0.5f,  0.5f,  0.5f}, {-1.0,  0.0,  0.0}, {1.0f, 0.0f}}, // top-right
        {{-0.5f, -0.5f,  0.5f}, {-1.0,  0.0,  0.0}, {0.0f, 0.0f}}, // bottom-right
        // Right face
        {{ 0.5f,  0.5f,  0.5f}, { 1.0,  0.0,  0.0}, {1.0f, 0.0f}}, // top-left
        {{ 0.5f,  0.5f, -0.5f}, { 1.0,  0.0,  0.0}, {1.0f, 1.0f}}, // top-right
        {{ 0.5f, -0.5f, -0.5f}, { 1.0,  0.0,  0.0}, {0.0f, 1.0f}}, // bottom-right
        {{ 0.5f, -0.5f, -0.5f}, { 1.0,  0.0,  0.0}, {0.0f, 1.0f}}, // bottom-right
        {{ 0.5f, -0.5f,  0.5f}, { 1.0,  0.0,  0.0}, {0.0f, 0.0f}}, // bottom-left
        {{ 0.5f,  0.5f,  0.5f}, { 1.0,  0.0,  0.0}, {1.0f, 0.0f}}, // top-left
        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, { 0.0, -1.0,  0.0}, {0.0f, 1.0f}}, // top-right
        {{ 0.5f, -0.5f,  0.5f}, { 0.0, -1.0,  0.0}, {1.0f, 0.0f}}, // bottom-left
        {{ 0.5f, -0.5f, -0.5f}, { 0.0, -1.0,  0.0}, {1.0f, 1.0f}}, // top-left
        {{ 0.5f, -0.5f,  0.5f}, { 0.0, -1.0,  0.0}, {1.0f, 0.0f}}, // bottom-left
        {{-0.5f, -0.5f, -0.5f}, { 0.0, -1.0,  0.0}, {0.0f, 1.0f}}, // top-right
        {{-0.5f, -0.5f,  0.5f}, { 0.0, -1.0,  0.0}, {0.0f, 0.0f}}, // bottom-right
        // Top face
        {{-0.5f,  0.5f, -0.5f}, { 0.0,  1.0,  0.0}, {0.0f, 1.0f}}, // top-left
        {{ 0.5f,  0.5f, -0.5f}, { 0.0,  1.0,  0.0}, {1.0f, 1.0f}}, // top-right
        {{ 0.5f,  0.5f,  0.5f}, { 0.0,  1.0,  0.0}, {1.0f, 0.0f}}, // bottom-right
        {{ 0.5f,  0.5f,  0.5f}, { 0.0,  1.0,  0.0}, {1.0f, 0.0f}}, // bottom-right
        {{-0.5f,  0.5f,  0.5f}, { 0.0,  1.0,  0.0}, {0.0f, 0.0f}}, // bottom-left
        {{-0.5f,  0.5f, -0.5f}, { 0.0,  1.0,  0.0}, {0.0f, 1.0f}}  // top-left
    };

    mesh->Indices = {
        // Back face
         0,  1,  2,  3,  4,  5,
        // Front face
         6,  7,  8,  9, 10, 11,
        // Left face
        12, 13, 14, 15, 16, 17,
        // Right face
        18, 19, 20, 21, 22, 23,
        // Bottom face
        24, 25, 26, 27, 28, 29,
        // Top face
        30, 31, 32, 33, 34, 35
    };

    // Loading surface
    auto surface = application.resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/container2.png");

    // Loading texture
    auto texture = new HG::Rendering::Base::Texture(surface);

    // Setting up texture
    application.renderer()->setup(texture);

    // Now surface can be set to null

    // Creating shader
    auto shader = new HG::Rendering::Base::Shader();

    shader->setShaderText(
        R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    vs_out.FragPos = inPosition;
    vs_out.Normal = inNormal;
    vs_out.TexCoords = inTexCoords;
}
#endif

#ifdef FragmentShader
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

in vec2 TexCoords;

uniform vec3 viewPos;

// texture samplers
uniform sampler2D diffuseTexture;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numberOfPointLights;

void main()
{
    //FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    //FragColor = texture(someTexture, TexCoords);
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

    // Ambient
    vec3 ambient = 0.05 * color;

    if (numberOfPointLights == 0)
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    // Diffuse
    vec3 lightDir = normalize(pointLights[0].position - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // Assuming bright white light color
    vec3 specular = vec3(0.3) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
#endif
)"
    );

    // Setting up shader
    application.renderer()->setup(shader);

    // Creating material
    auto material = new HG::Rendering::Base::Material();

    // Setting up material
    material->setShader(shader);
    material->set("diffuseTexture", texture);

    // Creating mesh renderer
    auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
    meshRenderer->setMesh(mesh);
    meshRenderer->setMaterial(material);

    // Setting up mesh renderer
    application.renderer()->setup(meshRenderer);

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
            .setGlobalPosition(glm::vec3(0, 0, 0))
    );

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Object")
            .addRenderingBehaviour(meshRenderer)
//            .addBehaviour(new RotationBehaviour)
            .setGlobalPosition(glm::vec3(0, 0, -2))
    );

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Light")
            .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
            .setGlobalPosition(glm::vec3(-1, 0.0, -1))
//            .addRenderingBehaviour(meshRenderer)
            .setScale(glm::vec3(0.2, 0.2, 0.2))
    );

    application.setScene(scene);

    return application.exec();
}