// Assets
#include <Assets/Scenes/TestScene.hpp>
#include <Assets/Materials/LightMaterial.hpp>
#include <Assets/Materials/PBRMaterial.hpp>
#include <Assets/Behaviours/LocalRotation.hpp>
#include <Assets/Behaviours/RotationBehaviour.hpp>

// HG::Core
#include <Application.hpp>
#include <GameObjectBuilder.hpp>
#include <TimeStatistics.hpp>

// HG::Rendering::Base
#include <MaterialCollection.hpp>
#include <Lights/PointLight.hpp>
#include <Behaviours/Mesh.hpp>
#include <Renderer.hpp>
#include <Texture.hpp>
#include <Camera.hpp>

// HG::Standard
#include <Behaviours/ServiceInformationOverlay.hpp>
#include <Behaviours/FPSCameraMovement.hpp>
#include <Behaviours/DebugControllerOverlay.hpp>

// HG::Utils
#include <Loaders/STBImageLoader.hpp>
#include <Loaders/AssimpLoader.hpp>
#include <Model.hpp>

void TestScene::start()
{
    // Loading model info
    auto model = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere.obj")
        .guaranteeGet();

    // Loading texture
    auto hdrTexture = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/HDR/Tokyo_BigSight_3k.hdr")
    );

    auto albedo = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/albedo.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear
    );

    auto metallic = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/metallic.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear
    );

    auto normal = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/normal.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear
    );

    auto roughness = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/GreasyPan/roughness.png"),
        HG::Rendering::Base::Texture::Filtering::Linear,
        HG::Rendering::Base::Texture::Filtering::Linear
    );

    auto material = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<PBRMaterial>();

    material->setAlbedoMap          (albedo);
    material->setMetallicMap        (metallic);
    material->setNormalMap          (normal);
    material->setRoughnessMap       (roughness);

    // Creating mesh renderer
    auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
    meshRenderer->setMesh(model->children()[0]->meshes()[0]);
    meshRenderer->setMaterial(material);

    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Object")
            .addBehaviour(meshRenderer)
            .addBehaviour(new LocalRotationBehaviour)
            .setGlobalPosition(glm::vec3(0.0f, 0.0f, 0.0f))
            .setScale({1.0f, 1.0f, 1.0f})
    );

    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
            .setGlobalPosition(glm::vec3(0, 0, 3))
    );

    auto lightParent = HG::Core::GameObjectBuilder()
        .setName("Lights")
        .setGlobalPosition(glm::vec3(0.0f, 0.0f, 10.0))
        .deploy();

    addGameObject(lightParent);

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(
            HG::Core::GameObjectBuilder()
                .setName("Light Top Right")
                .setParent(lightParent)
                .setLocalPosition(glm::vec3(10.0, 10.0, 0.0))
                .addBehaviour(lightRenderer)
                .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                .setScale(glm::vec3(0.1, 0.1, 0.1))
        );
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(
            HG::Core::GameObjectBuilder()
                .setName("Light Top Left")
                .setParent(lightParent)
                .setLocalPosition(glm::vec3(-10.0, 10.0, 0.0))
                .addBehaviour(lightRenderer)
                .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                .setScale(glm::vec3(0.1, 0.1, 0.1))
        );
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(
            HG::Core::GameObjectBuilder()
                .setName("Light Bot Left")
                .setParent(lightParent)
                .setLocalPosition(glm::vec3(-10.0, -10.0, 0.0))
                .addBehaviour(lightRenderer)
                .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                .setScale(glm::vec3(0.1, 0.1, 0.1))
        );
    }

    {
        auto lightRenderer = new HG::Rendering::Base::Behaviours::Mesh;
        lightRenderer->setMesh(model->children()[0]->meshes()[0]);

        addGameObject(
            HG::Core::GameObjectBuilder()
                .setName("Light Bot Right")
                .setParent(lightParent)
                .setLocalPosition(glm::vec3(10.0, -10.0, 0.0))
                .addBehaviour(lightRenderer)
                .addBehaviour(new HG::Rendering::Base::Lights::PointLight)
                .setScale(glm::vec3(0.1, 0.1, 0.1))
        );
    }

}
