// RenderToTexture
#include <Assets/Scenes/RenderToTextureScene.hpp>
#include <Assets/Materials/TextureMaterial.hpp>

// HG::Core
#include <Behaviour.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <MaterialCollection.hpp>
#include <Texture.hpp>

// HG::Utils
#include <Loaders/AssimpLoader.hpp>

class RenderToTextureBehaviour : public HG::Core::Behaviour
{
public:

    void update() override
    {
        Behaviour::update();
    }

};

void RenderToTextureScene::start()
{
    // Loading model
    auto cubeModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/cube.obj");

    // Material
    auto material = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<TextureMaterial>();


    // Creating texture
    auto texture = new HG::Rendering::Base::Texture(
        {200, 200}
    );

//    material->setTexture()

}
