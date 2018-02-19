#include <Application.hpp>
#include <Scene.hpp>
#include <iostream>
#include <thread>
#include <GameObjectBuilder.hpp>
#include <Camera.hpp>
#include "Behaviour.hpp"


int main(int argc, char** argv)
{
    HG::Core::Application application(argc, argv);

    application.resourceManager()->setResourceAccessor<>();

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