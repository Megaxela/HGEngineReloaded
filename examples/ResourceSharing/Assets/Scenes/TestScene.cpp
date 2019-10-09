// Resource Sharing example
#include <Assets/Scenes/TestScene.hpp>
#include <Assets/Materials/TextureMaterial.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/ResourceManager.hpp>
#include <HG/Core/ResourceCache.hpp>
#include <HG/Core/GameObjectBuilder.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/ServiceInformationOverlay.hpp>
#include <HG/Standard/Behaviours/FPSCameraMovement.hpp>
#include <HG/Standard/Behaviours/DebugControllerOverlay.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Model.hpp>

// GLM
#include <glm/gtx/quaternion.hpp>

// ImGui
#include <imgui.h>

class DescriptionBehaviour : public HG::Core::Behaviour
{
public:
    DescriptionBehaviour()
    {}

protected:
    void onUpdate() override
    {
        if (ImGui::Begin("Description"))
        {
            ImGui::Text(
                "This example shows engine ability to\n"
                "share resources. All this models must use\n"
                "same buffers for rendering."
            );

            ImGui::End();
        }
    }
};

void TestScene::start()
{
    // Loading texture image
    auto textureImage = application()->resourceManager()->load<HG::Utils::STBImageLoader>("Assets/Textures/texture.png");

    // Creating texture
    auto texture = registerResource(new (application()->resourceCache()) HG::Rendering::Base::Texture(
            textureImage, HG::Rendering::Base::Texture::Filtering::Linear, HG::Rendering::Base::Texture::Filtering::Linear));

    // Loading cube model
    auto cube = application()->resourceManager()->load<HG::Utils::AssimpLoader>("Assets/Models/cube.obj");

    // Creating material
    auto material = registerResource(application()->renderer()->materialCollection()->getMaterial<TextureMaterial>());

    material->setTexture(texture);

    // Adding camera
    addGameObject(
            HG::Core::GameObjectBuilder(application()->resourceCache())
                .setName("Camera")
                .addBehaviour(new DescriptionBehaviour())
                .addBehaviour(new HG::Rendering::Base::Camera)
                .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
                .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
                .addBehaviour(new HG::Standard::Behaviours::DebugControllerOverlay)
                .setGlobalPosition(glm::vec3(15.5, 2.0, 12.0))
                .setRotation(glm::quat(glm::vec3(glm::radians(-27.0f), 0.0f, 0.0f))));

    // Creating N objects
    for (uint32_t ix = 0; ix < 32; ++ix)
    {
        for (uint32_t iy = 0; iy < 32; ++iy)
        {
            addGameObject(HG::Core::GameObjectBuilder(application()->resourceCache())
                            .setGlobalPosition({ix, iy, 0})
                            .setScale(glm::vec3(0.5, 0.5, 0.5))
                            .addBehaviour(new HG::Rendering::Base::Behaviours::Mesh(cube.guaranteeGet()->children()[0]->meshes()[0], material)));
        }
    }
}
