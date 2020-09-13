// HG::Core
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>
#include <HG/Rendering/OpenGL/Common/RenderTargetDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderDataProcessor.hpp>

// HG::Rendering::Vulkan
#include <HG/Rendering/Vulkan/RenderingPipeline.hpp>

// HG::System::PC
#include <HG/System/PC/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

int main(int argc, char** argv)
{
    HGInfo("Creating application");
    HG::Core::Application application("HGEngine Minimal Rendering Example", argc, argv);

    // Setting system controller
    application.setSystemController(new HG::System::PC::SystemController(&application));

    // Setting empty pipeline
//    auto pipeline = new HG::Rendering::OpenGL::Forward::RenderingPipeline(&application);
//    // todo: move to rendering pipeline as required data processors (cause of derived renderers)
//    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::RenderTargetDataProcessor);
//    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);
//    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor);
    auto pipeline = new HG::Rendering::Vulkan::RenderingPipeline(&application);

    application.renderer()->setPipeline(pipeline);

    if (!application.init())
    {
        HGError("Can't init application");
        return -1;
    }

    return application.exec();
}