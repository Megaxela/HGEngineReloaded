// Orbital example
#include <Assets/Behaviours/LocalRotation.hpp>
#include <Assets/Materials/PlainMaterial.hpp>
#include <Assets/Scenes/OrbitalScene.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/GameObjectBuilder.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/CubeMap.hpp>
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/CubeMap.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/DebugControllerOverlay.hpp>
#include <HG/Standard/Behaviours/FPSCameraMovement.hpp>

// HG::Utils
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Model.hpp>

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
                "Scene is described in \"Assets/Scenes/OribitalScene\".");
        }
        ImGui::End();
    }
};

void OrbitalScene::start()
{
    // Loading model
    auto sphereModel =
        application()->resourceManager()->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere.obj").guaranteeGet();

    // Creating cubemap object
    // and loading cubemap textures
    // (async loading)
    auto cubemap = registerResource(new (application()->resourceCache()) HG::Rendering::Base::CubeMap(
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_ft.tga"),
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_bk.tga"),
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_up.tga"),
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_dn.tga"),
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_lf.tga"),
        application()->resourceManager()->load<HG::Utils::STBImageLoader>(
            "Assets/Cubemaps/PurpleNebula/purplenebula_rt.tga")));

    // Creating materials
    auto firstMaterial =
        registerResource(application()->renderer()->materialCollection()->getMaterial<PlainMaterial>());

    auto secondMaterial =
        registerResource(application()->renderer()->materialCollection()->getMaterial<PlainMaterial>());

    auto thirdMaterial =
        registerResource(application()->renderer()->materialCollection()->getMaterial<PlainMaterial>());

    firstMaterial->setColor(HG::Utils::Color::Red);
    secondMaterial->setColor(HG::Utils::Color::Green);
    thirdMaterial->setColor(HG::Utils::Color::Blue);

    // Placing camera
    addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                      .setName("Camera")
                      .setGlobalPosition({-3.0f, 2.5f, 3.0f})
                      .setRotation(glm::quat(glm::vec3(0.7f, 0.6f, 0.5f))) // Euler with radians
                      .addBehaviour(new HG::Rendering::Base::Camera)
                      .addBehaviour(new HG::Rendering::Base::Behaviours::CubeMap(cubemap))
                      .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
                      .addBehaviour(new DescriptionBehaviour));

    // Creating biggest (Red) sphere
    auto firstParent = HG::Core::GameObjectBuilder(application()->resourceCache())
                           .setName("Big Red Planet")
                           .setGlobalPosition({0.0f, 0.0f, 0.0f})
                           .addBehaviour(new LocalRotationBehaviour(4.0f))
                           .addBehaviour(new HG::Rendering::Base::Behaviours::Mesh(
                               sphereModel->children()[0]->meshes()[0], firstMaterial))
                           .deploy();

    // Creating smaller (Green) sphere
    auto secondParent = HG::Core::GameObjectBuilder(application()->resourceCache())
                            .setName("Smaller Green Planet")
                            .setParent(firstParent)
                            .setGlobalPosition({0.0f, 0.0f, 2.0f})
                            .setScale({0.5f, 0.5f, 0.5f})
                            .addBehaviour(new LocalRotationBehaviour(2.0f))
                            .addBehaviour(new HG::Rendering::Base::Behaviours::Mesh(
                                sphereModel->children()[0]->meshes()[0], secondMaterial))
                            .deploy();

    // Creating smallest (Blue) sphere
    auto thirdParent = HG::Core::GameObjectBuilder(application()->resourceCache())
                           .setName("Smallest Blue Planet")
                           .setParent(secondParent)
                           .setGlobalPosition({0.0f, 0.0f, 2.7f})
                           .setScale({0.3f, 0.3f, 0.3f})
                           .addBehaviour(new LocalRotationBehaviour(1.0f))
                           .addBehaviour(new HG::Rendering::Base::Behaviours::Mesh(
                               sphereModel->children()[0]->meshes()[0], thirdMaterial))
                           .deploy();

    addGameObject(firstParent);
    addGameObject(secondParent);
    addGameObject(thirdParent);
}
