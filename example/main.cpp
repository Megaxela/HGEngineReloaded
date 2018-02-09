#include <Application.hpp>
#include <Scene.hpp>
#include <iostream>
#include <thread>
#include <GameObjectBuilder.hpp>
#include "Behaviour.hpp"

class TestBH : public HG::Core::Behaviour
{
public:
protected:

};

int main(int argc, char** argv)
{
    HG::Core::Application application(argc, argv);

    auto* scene = new HG::Core::Scene();

    scene->addGameObject(HG::Core::GameObjectBuilder::i().addBehaviour(new TestBH()));
    scene->addGameObject(HG::Core::GameObjectBuilder::i());
    scene->addGameObject(HG::Core::GameObjectBuilder::i());
    scene->addGameObject(HG::Core::GameObjectBuilder::i());

    application.setScene(scene);

    return application.exec();
}