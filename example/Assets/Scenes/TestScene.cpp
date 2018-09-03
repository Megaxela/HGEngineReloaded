#include <Assets/Scenes/TestScene.hpp>
#include <Loaders/STBImageLoader.hpp>
#include <Loaders/AssimpLoader.hpp>
#include <Behaviours/ServiceInformationOverlay.hpp>
#include <Behaviours/FPSCameraMovement.hpp>
#include <Behaviours/Mesh.hpp>
#include <Lights/PointLight.hpp>
#include <GameObjectBuilder.hpp>
#include <Application.hpp>
#include <Texture.hpp>
#include <Model.hpp>

#include <Assets/Materials/LightMaterial.hpp>
#include <Assets/Materials/PBRMaterial.hpp>

#include <Assets/Behaviours/LocalRotation.hpp>
#include <Assets/Behaviours/RotationBehaviour.hpp>
#include <Behaviours/DebugControllerOverlay.hpp>

void TestScene::start()
{
    // Loading model info
    auto model = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/icosphere_hq.obj");

    // Loading texture

//    auto hdrTexture = new HG::Rendering::Base::Texture(
//        application()->resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/HDR/Tokyo_BigSight_3k.hdr")
//    );

    auto albedo = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/albedo.png")
    );

    auto ao = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/ao.png")
    );

    auto height = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/height.png")
    );

    auto metallic = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/metallic.png")
    );

    auto normal = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/normal.png")
    );

    auto roughness = new HG::Rendering::Base::Texture(
        application()->resourceManager()
            ->load<HG::Utils::STBImageLoader>("Assets/PBR/CopperRock/roughness.png")
    );

    auto material = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<PBRMaterial>();

    material->setAlbedoMap          (albedo);
    material->setAmbientOcclusionMap(ao);
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

//    auto parent = HG::Core::GameObjectBuilder()
//        .setName("Objects")
//        .deploy();
//
//    addGameObject(parent);
//
//    for (int ix = 0; ix < 7; ++ix)
//    {
//        for (int iy = 0; iy < 7; ++iy)
//        {
//            auto material = application()
//                ->renderer()
//                ->materialCollection()
//                ->getMaterial<PBRMaterial>();
//
//            material->setAlbedoMap          (albedo);
//            material->setAmbientOcclusionMap(ao);
//            material->setMetallicMap        (metallic);
//            material->setNormalMap          (normal);
//            material->setRoughnessMap       (roughness);
//
////            material->setAlbedo(HG::Utils::Color(0.5f, 0.0f, 0.0f));
////            material->setMetallic(float(iy) / 7.0f);
////            material->setRoughness(glm::clamp(float(ix) / 7.0f, 0.05f, 1.0f));
////            material->set("ao", 1.0f);
//
////            Info() << "(" << ix << ", " << iy << "): m: " << (1.0 / 6.0 * iy) << ", r: " << (1.0 / 6.0 * ix);
//
////            material->setDiffuse(texture);
//
//            // Creating mesh renderer
//            auto meshRenderer = new HG::Rendering::Base::Behaviours::Mesh;
//            meshRenderer->setMesh(model->children()[0]->meshes()[0]);
//            meshRenderer->setMaterial(material);
//
//            addGameObject(
//                HG::Core::GameObjectBuilder()
//                    .setName("Object " + std::to_string(ix) + " " + std::to_string(iy))
//                    .addBehaviour(meshRenderer)
//                    .addBehaviour(new LocalRotationBehaviour)
//                    .setGlobalPosition(glm::vec3(-3.0f + ix, -3.0f + iy, 0))
//                    .setScale({0.3f, 0.3f, 0.3f})
//                    .setParent(parent)
//            );
//        }
//    }

    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
            .setGlobalPosition(glm::vec3(0, 0, 0))
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
