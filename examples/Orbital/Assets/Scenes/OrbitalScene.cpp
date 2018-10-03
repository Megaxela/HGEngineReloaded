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
#include <CubeMap.hpp>
#include <Behaviours/CubeMap.hpp>

// HG::Rendering::OpenGL
#include <Behaviours/Mesh.hpp>

// HG::Standard
#include <Behaviours/DebugControllerOverlay.hpp>

// HG::Utils
#include <Loaders/AssimpLoader.hpp>
#include <Model.hpp>
#include <Behaviours/FPSCameraMovement.hpp>
#include <Loaders/STBImageLoader.hpp>

// GLM
#include <glm/gtx/quaternion.hpp>

// ImGui
#include <imgui.h>

class DescriptionBehaviour : public HG::Core::Behaviour
{
protected:

    void onUpdate() override
    {
        if (ImGui::Begin("Description"))
        {
            ImGui::Text(
                "This example shows gameobject parent system.\n"
                "Scene is described in \"Assets/Scenes/OribitalScene\"."
            );
            ImGui::End();
        }
    }
};

void OrbitalScene::start()
{
    // Loading model
    auto sphereModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere.obj")
        .guaranteeGet();

    // Creating cubemap object
    // and loading cubemap textures
    // (async loading)
    auto cubemap = new HG::Rendering::Base::CubeMap(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_ft.tga"),
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_bk.tga"),
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_up.tga"),
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_dn.tga"),
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_lf.tga"),
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/Cubemaps/PurpleNebula/purplenebula_rt.tga")
    );

    // Creating materials
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
            .addBehaviour(new HG::Rendering::Base::Behaviours::CubeMap(cubemap))
            .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
            .addBehaviour(new DescriptionBehaviour)
    );

    // Creating biggest (Red) sphere
    auto firstParent = HG::Core::GameObjectBuilder()
        .setName("Big Red Planet")
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
        .setName("Smaller Green Planet")
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
        .setName("Smallest Blue Planet")
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