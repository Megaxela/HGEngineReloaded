// RenderToTexture
#include <Assets/Scenes/RenderToTextureScene.hpp>
#include <Assets/Materials/TextureMaterial.hpp>
#include <Assets/Behaviours/RenderToTextureBehaviour.hpp>

// HG::Core
#include <Behaviour.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GameObjectBuilder.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <MaterialCollection.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Behaviours/Mesh.hpp>

// HG::Standard
#include <Behaviours/FPSCameraMovement.hpp>

// HG::Utils
#include <Loaders/AssimpLoader.hpp>
#include <Model.hpp>

// GLM
#include <glm/gtx/quaternion.hpp>

void RenderToTextureScene::start()
{
    // Loading model
    auto cubeModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/cube.obj")
        .guaranteeGet();


    // Creating texture
    auto texture = new HG::Rendering::Base::Texture(
        {200, 200},
        HG::Rendering::Base::Texture::Format::RGBA
    );

    // Material
    auto material = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<TextureMaterial>();

    material->setTexture(texture);

    // Creation mesh behaviour
    auto meshRenderingBehaviour = new HG::Rendering::Base::Behaviours::Mesh(
        cubeModel->children()[0]->meshes()[0],
        material
    );

    // Creating behaviour
    auto renderToTexture = new RenderToTextureBehaviour();

    renderToTexture->setTarget(texture);
    renderToTexture->setRenderBehaviour(meshRenderingBehaviour);

    // Creating camera
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setGlobalPosition({0, 0, 2})
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
        );

    // Creating object
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setGlobalPosition({0, 0, 0})
            .setRotation(glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)))
            .addBehaviour(meshRenderingBehaviour)
            .addBehaviour(renderToTexture)
    );
}
