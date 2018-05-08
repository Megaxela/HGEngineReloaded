#include <Application.hpp>
#include <Scene.hpp>
#include <iostream>
#include <thread>
#include <GameObjectBuilder.hpp>
#include <Camera.hpp>
#include <Loggers/BasicLogger.hpp>
#include <Mesh.hpp>
#include <Behaviours/Mesh.hpp>
#include "Behaviour.hpp"
#include "ForwardRenderingPipeline.hpp"
#include "GLFWSystemController.hpp"

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    HG::Core::Application application(argc, argv);

    application.renderer()
        ->setPipeline<HG::Rendering::OpenGL::ForwardRenderingPipeline>();

    application.setSystemController<HG::Rendering::OpenGL::GLFWSystemController>();

    auto* scene = new HG::Core::Scene();

    // Initial mesh
    HG::Utils::MeshPtr mesh = std::make_shared<HG::Utils::Mesh>();

    mesh->Vertices = {
        // position            // uv
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}, // top right
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // bottom right
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // bottom left
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}
    };

    mesh->Indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Setting up mesh renderer
    auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
    meshRenderer->setMesh(mesh);

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .addBehaviour(new HG::Rendering::Base::Camera)
    );

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Object")
            .addRenderingBehaviour(meshRenderer)
            .setGlobalPosition(glm::vec3(1, 1, 1))
    );

    scene->addGameObject(HG::Core::GameObjectBuilder());
    scene->addGameObject(HG::Core::GameObjectBuilder());

    application.setScene(scene);

    return application.exec();
}