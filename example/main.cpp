#include <Application.hpp>
#include <Scene.hpp>
#include <iostream>
#include <thread>
#include <GameObjectBuilder.hpp>
#include <Camera.hpp>
#include <Loggers/BasicLogger.hpp>
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

    scene->addGameObject(
        HG::Core::GameObjectBuilder()
            .addBehaviour(new HG::Rendering::Base::Camera)
    );

    scene->addGameObject(HG::Core::GameObjectBuilder());
    scene->addGameObject(HG::Core::GameObjectBuilder());
    scene->addGameObject(HG::Core::GameObjectBuilder());

    application.setScene(scene);

    return application.exec();
}