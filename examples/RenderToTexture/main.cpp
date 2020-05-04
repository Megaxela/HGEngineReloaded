// RenderToTexture
#include <Assets/Scenes/RenderToTextureScene.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/FilesystemResourceAccessor.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Common/MeshDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/RenderTargetDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Forward/MeshRenderer.hpp>
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>

// HG::System::PC
#include <HG/System/PC/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

int main(int argc, char** argv)
{
    HGInfo("Creating application");
    HG::Core::Application application("HGEngine RenderToTexture Example", argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()->setResourceAccessor(new HG::Core::FilesystemResourceAccessor());

    // Setting system controller
    application.setSystemController(new HG::System::PC::SystemController(&application));

    auto pipeline = new HG::Rendering::OpenGL::Forward::RenderingPipeline(&application);
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::MeshRenderer());
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::RenderTargetDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::MeshDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);

    // Setting rendering to forward
    application.renderer()->setPipeline(pipeline);

    if (!application.init())
    {
        HGError("Can't init application.");
        return -1;
    }

    application.setScene(new RenderToTextureScene());

    return application.exec();
}
