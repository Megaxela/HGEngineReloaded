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
                "This example shows rendering to texture ability.\n"
                "Cube is just renders again in separate framebuffer with\n"
                "current camera. And this framebuffers texture is applied to\n"
                "cube. (to show different recursion levels, shader change color\n"
                "of texture a little).\n"
                "Recursion rendering was bypassed by using 2 textures\n"
                "(like in double buffering).\n"
                "You are able to enable FPS camera movement with `R`."
            );
            ImGui::End();
        }
    }
};

void RenderToTextureScene::start()
{
    // Loading model
    auto cubeModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("Assets/Models/cube.obj")
        .guaranteeGet();


    // Creating textures for recursion
    auto texture1 = new HG::Rendering::Base::Texture(
        {400, 400},
        HG::Rendering::Base::Texture::Format::RGBA
    );

    auto texture2 = new HG::Rendering::Base::Texture(
        {400, 400},
        HG::Rendering::Base::Texture::Format::RGBA
    );

    // Material
    auto material = application()
        ->renderer()
        ->materialCollection()
        ->getMaterial<TextureMaterial>();

    material->setTexture(texture1);

    // Creation mesh behaviour
    auto meshRenderingBehaviour = new HG::Rendering::Base::Behaviours::Mesh(
        cubeModel->children()[0]->meshes()[0],
        material
    );

    // Creating behaviour
    auto renderToTexture = new RenderToTextureBehaviour();

    renderToTexture->setTarget1(texture1);
    renderToTexture->setTarget2(texture2);
    renderToTexture->setRenderBehaviour(meshRenderingBehaviour);

    // Creating camera
    addGameObject(
        HG::Core::GameObjectBuilder()
            .setGlobalPosition({0, 0, 2})
            .addBehaviour(new DescriptionBehaviour)
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
