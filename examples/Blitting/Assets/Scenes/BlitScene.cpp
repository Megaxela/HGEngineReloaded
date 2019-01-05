// Blitting example
#include <Assets/Scenes/BlitScene.hpp>
#include <Assets/Materials/TextureMaterial.hpp>

// HG::Core
#include <HG/Core/Behaviour.hpp>
#include <HG/Core/ResourceManager.hpp>
#include <HG/Core/GameObjectBuilder.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/BlitData.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/ServiceInformationOverlay.hpp>
#include <HG/Standard/Behaviours/FPSCameraMovement.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Model.hpp>

// ImGui
#include <imgui.h>

// GLM
#include <glm/gtx/quaternion.hpp>

class DescriptionBehaviour : public HG::Core::Behaviour
{
public:

    DescriptionBehaviour(HG::Rendering::Base::Texture* baseTexture,
                         HG::Rendering::Base::Texture* resultTexture) :
        m_baseTexture(baseTexture),
        m_resultTexture(resultTexture)
    {}

protected:

    void onUpdate() override
    {
        if (ImGui::Begin("Description"))
        {
            ImGui::Text(
                    "This example shows engine blitting ability.\n"
                    "It uses 2 textures, one of them is texture atlas.\n"
                    "second one is destination. "
            );

            ImGui::Text("Source: ");
            ImGui::SameLine();
            ImGui::Image(m_baseTexture, {256, 256});

            ImGui::SameLine();

            ImGui::Text("Result: ");
            ImGui::SameLine();
            ImGui::Image(m_resultTexture, {256, 256});

            ImGui::End();
        }
    }

private:
    HG::Rendering::Base::Texture* m_baseTexture;
    HG::Rendering::Base::Texture* m_resultTexture;
};

void BlitScene::start()
{
    // Loading atlas texture
    auto surface = application()->resourceManager()
        ->load<HG::Utils::STBImageLoader>("Assets/Textures/atlas.png");

    auto atlasTexture = registerResource(
        new (application()->resourceCache()) HG::Rendering::Base::Texture(
            surface,
            HG::Rendering::Base::Texture::Filtering::Nearest,
            HG::Rendering::Base::Texture::Filtering::Nearest
        )
    );

    // Loading cube model
    auto cube = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/cube.obj");

    // Creating target texture
    auto targetTexture = registerResource(
        new (application()->resourceCache()) HG::Rendering::Base::Texture(
            {256, 256},
            HG::Rendering::Base::Texture::Format::RGB
        )
    );

    auto targetRendertarget = registerResource(
        new (application()->resourceCache()) HG::Rendering::Base::RenderTarget(
            {256, 256}
        )
    );
    targetRendertarget->setColorTexture(targetTexture);

    // Preparing blitting
    auto tileSize = 256 / 16;
    HG::Rendering::Base::BlitData blitData;

    // Filling texture with "grass" tile
    for (uint32_t ix = 0; ix < targetTexture->size().x / tileSize; ++ix)
    {
        for (uint32_t iy = 0; iy < targetTexture->size().y / tileSize; ++iy)
        {
            blitData.blitRectangular(
                atlasTexture, // Texture
                {tileSize * 3, 0}, // Top left
                {tileSize * 4, tileSize}, // Bottom right
                {ix * tileSize, iy * tileSize} // Position in target
            );
        }
    }

    // Creating material
    auto material = registerResource(
        application()
            ->renderer()
            ->materialCollection()
            ->getMaterial<TextureMaterial>()
    );

    material->setTexture(targetTexture);

    // Creating mesh behaviour
    auto meshRenderingBehaviour = new HG::Rendering::Base::Behaviours::Mesh(
        cube.guaranteeGet()->children()[0]->meshes()[0],
        material
    );

    // Wait for atlas texture
    surface.guaranteeGet();

    // Rendering into target texture
    application()
        ->renderer()
        ->pipeline()
        ->blit(targetRendertarget, &blitData);

    // Adding camera
    addGameObject(
        HG::Core::GameObjectBuilder(application()->resourceCache())
            .setName("Camera")
            .addBehaviour(new DescriptionBehaviour(atlasTexture, targetTexture))
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
            .setGlobalPosition(glm::vec3(0, 0, 3))
    );

    // Creating object
    addGameObject(
        HG::Core::GameObjectBuilder(application()->resourceCache())
            .setGlobalPosition({0, 0, 0})
            .setRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f)))
            .addBehaviour(meshRenderingBehaviour)
    );
}
