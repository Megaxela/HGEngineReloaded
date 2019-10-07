// PBR example
#include <Assets/Behaviours/RotationBehaviour.hpp>
#include <Assets/Materials/LightMaterial.hpp>
#include <Assets/Materials/PBRMaterial.hpp>
#include <Assets/Scenes/TestScene.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/GameObjectBuilder.hpp>
#include <HG/Core/TimeStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Lights/PointLight.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Texture.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/DebugControllerOverlay.hpp>
#include <HG/Standard/Behaviours/FPSCameraMovement.hpp>
#include <HG/Standard/Behaviours/ServiceInformationOverlay.hpp>

// HG::Utils
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Model.hpp>

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
                "This example shows PBR material example.\n"
                "Actual PBR material is implemented in custom "
                "material class (\"Assets/Materials/PBRMaterial\").\n"
                "All scene setup is performed in (\"Assets/Scenes/TestScene\").\n"
                "You are able to enable FPS camera movement with `R`.");
        }
        ImGui::End();
    }
};

void FirstScene::start()
{
    // Loading model info
    auto model =
        application()->resourceManager()->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere.obj").guaranteeGet();

    // Loading texture
    //    auto hdrTexture = new HG::Rendering::Base::Texture(
    //        application()->resourceManager()
    //            ->load<HG::Utils::STBImageLoader>("Assets/HDR/Tokyo_BigSight_3k.hdr")
    //    );

    auto albedo = registerResource(new (application()->resourceCache()) HG::Rendering::Base::Texture(
        application()->resourceManager()->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/albedo.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear));

    auto metallic = registerResource(new (application()->resourceCache()) HG::Rendering::Base::Texture(
        application()->resourceManager()->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/metallic.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear));

    auto normal = registerResource(new (application()->resourceCache()) HG::Rendering::Base::Texture(
        application()->resourceManager()->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/normal.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear));

    auto roughness = registerResource(new (application()->resourceCache()) HG::Rendering::Base::Texture(
        application()->resourceManager()->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/roughness.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear));

    auto material = registerResource(application()->renderer()->materialCollection()->getMaterial<PBRMaterial>());

    material->setAlbedoMap(albedo);
    material->setMetallicMap(metallic);
    material->setNormalMap(normal);
    material->setRoughnessMap(roughness);

    // Creating mesh renderer
    auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
    meshRenderer->setMesh(model->children()[0]->meshes()[0]);
    meshRenderer->setMaterial(material);

    addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                      .setName("Object")
                      .addBehaviour(meshRenderer)
                      .setGlobalPosition(glm::vec3(0.0f, 0.0f, 0.0f))
                      .setScale({1.0f, 1.0f, 1.0f}));

    addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                      .setName("Camera")
                      .addBehaviour(new DescriptionBehaviour)
                      .addBehaviour(new HG::Rendering::Base::Camera)
                      .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
                      .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
                      .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
                      .setGlobalPosition(glm::vec3(0, 0, 3)));

    auto lightParent = HG::Core::GameObjectBuilder(application()->resourceCache())
                           .setName("Lights")
                           .setGlobalPosition(glm::vec3(0.0f, 0.0f, 10.0))
                           .deploy();

    addGameObject(lightParent);

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                          .setName("Light Top Right")
                          .setParent(lightParent)
                          .setLocalPosition(glm::vec3(10.0, 10.0, 0.0))
                          .addBehaviour(lightRenderer)
                          .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                          .setScale(glm::vec3(0.1, 0.1, 0.1)));
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                          .setName("Light Top Left")
                          .setParent(lightParent)
                          .setLocalPosition(glm::vec3(-10.0, 10.0, 0.0))
                          .addBehaviour(lightRenderer)
                          .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                          .setScale(glm::vec3(0.1, 0.1, 0.1)));
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                          .setName("Light Bot Left")
                          .setParent(lightParent)
                          .setLocalPosition(glm::vec3(-10.0, -10.0, 0.0))
                          .addBehaviour(lightRenderer)
                          .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                          .setScale(glm::vec3(0.1, 0.1, 0.1)));
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                          .setName("Light Bot Right")
                          .setParent(lightParent)
                          .setLocalPosition(glm::vec3(10.0, -10.0, 0.0))
                          .addBehaviour(lightRenderer)
                          .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                          .setScale(glm::vec3(0.1, 0.1, 0.1)));
    }
}
