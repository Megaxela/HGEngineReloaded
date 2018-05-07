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

    // Preparing mesh for rendering


    // Setting up mesh renderer
    auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
//    meshRenderer->setMesh();

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