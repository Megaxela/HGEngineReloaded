// Orbital example
#include <Assets/Scenes/OrbitalScene.hpp>
#include <Assets/Materials/PlainMaterial.hpp>
#include <Assets/Behaviours/LocalRotation.hpp>

// HG::Core
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <GameObjectBuilder.hpp>

// HG::Rendering::Base
#include <MaterialCollection.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>

// HG::Rendering::OpenGL
#include <Behaviours/Mesh.hpp>

// HG::Utils
#include <Loaders/AssimpLoader.hpp>
#include <Model.hpp>
#include <Behaviours/FPSCameraMovement.hpp>

// GLM
#include <glm/gtx/quaternion.hpp>


void OrbitalScene::start()
{
    // Loading model
    auto sphereModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere.obj")
        .guaranteeGet();

    // Center material
    auto firstMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<PlainMaterial>();

    auto secondMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<PlainMaterial>();

    auto thirdMaterial = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<PlainMaterial>();


    firstMaterial->setColor(HG::Utils::Color::Red);
    secondMaterial->setColor(HG::Utils::Color::Green);
    thirdMaterial->setColor(HG::Utils::Color::Blue);

    // Placing camera
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .setGlobalPosition({-3.0f, 2.5f, 3.0f})
            .setRotation(glm::quat(glm::vec3(0.7f, 0.6f, 0.5f))) // Euler with radians
            .addBehaviour(new HG::Rendering::Base::Camera)
    );

    // Creating biggest (Red) sphere
    auto firstParent = HG::Core::GameObjectBuilder()
        .setGlobalPosition({0.0f, 0.0f, 0.0f})
        .addBehaviour(
            new LocalRotationBehaviour(4.0f)
        ).addBehaviour(
            new HG::Rendering::Base::Behaviours::Mesh(
                sphereModel->children()[0]->meshes()[0],
                firstMaterial
            )
        ).deploy();

    // Creating smaller (Green) sphere
    auto secondParent = HG::Core::GameObjectBuilder()
        .setParent(firstParent)
        .setGlobalPosition({0.0f, 0.0f, 2.0f})
        .setScale({0.5f, 0.5f, 0.5f})
        .addBehaviour(
            new LocalRotationBehaviour(2.0f)
        ).addBehaviour(
            new HG::Rendering::Base::Behaviours::Mesh(
                sphereModel->children()[0]->meshes()[0],
                secondMaterial
            )
        ).deploy();

    // Creating smallest (Blue) sphere
    auto thirdParent = HG::Core::GameObjectBuilder()
        .setParent(secondParent)
        .setGlobalPosition({0.0f, 0.0f, 2.7f})
        .setScale({0.3f, 0.3f, 0.3f})
        .addBehaviour(
            new LocalRotationBehaviour(1.0f)
        ).addBehaviour(
            new HG::Rendering::Base::Behaviours::Mesh(
                sphereModel->children()[0]->meshes()[0],
                thirdMaterial
            )
        ).deploy();


    addGameObject(firstParent);
    addGameObject(secondParent);
    addGameObject(thirdParent);
}
