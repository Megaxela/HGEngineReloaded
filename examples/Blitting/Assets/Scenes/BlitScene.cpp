// Blitting example
#include <Assets/Scenes/BlitScene.hpp>

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

// HG::Standard
#include <HG/Standard/Behaviours/ServiceInformationOverlay.hpp>
#include <HG/Standard/Behaviours/FPSCameraMovement.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>

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
                    "This example shows engine blitting ability.\n"
                    "It uses 2 textures, one of them is texture atlas.\n"
                    "second one is destination. "
            );
        }
    }
};

void BlitScene::start()
{
    // Loading atlas texture
    auto atlasTexture = registerResource(
        new HG::Rendering::Base::Texture(
            application()->resourceManager()
                ->load<HG::Utils::STBImageLoader>("Assets/Textures/atlas.png"),
            HG::Rendering::Base::Texture::Filtering::Nearest,
            HG::Rendering::Base::Texture::Filtering::Nearest
        )
    );

    // Creating target texture
    auto targetTexture = registerResource(
        new HG::Rendering::Base::Texture({1024, 1024}, HG::Rendering::Base::Texture::Format::RGB)
    );

    auto targetRendertarget = registerResource(
        new HG::Rendering::Base::RenderTarget({1024, 1024})
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
                atlasTexture,
                {0, 0},
                {tileSize, tileSize},
                {ix * tileSize, iy * tileSize}
            );
        }
    }

    // Rendering into target texture
    application()
        ->renderer()
        ->pipeline()
        ->blit(targetRendertarget, &blitData);

    // Adding camera
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setName("Camera")
            .addBehaviour(new DescriptionBehaviour)
            .addBehaviour(new HG::Rendering::Base::Camera)
            .addBehaviour(new HG::Standard::Behaviours::ServiceInformationOverlay)
            .addBehaviour(new HG::Standard::Behaviours::FPSCameraMovement)
            .setGlobalPosition(glm::vec3(0, 0, 3))
    );

    // todo: Add texture display
}
