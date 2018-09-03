#include <Assets/Scenes/TestScene.hpp>
#include <CurrentLogger.hpp>
#include <Loggers/BasicLogger.hpp>
#include <Application.hpp>
#include <FilesystemResourceAccessor.hpp>
#include <GLFWSystemController.hpp>
#include <Forward/RenderingPipeline.hpp>
#include <Forward/MeshRenderer.hpp>
#include <Common/MeshDataProcessor.hpp>
#include <Common/Texture2DDataProcessor.hpp>
#include <Common/ShaderDataProcessor.hpp>


//HG::Rendering::Base::CubeMapTexture* loadCubemap(HG::Core::Application& application)
//{
//    auto cubeMap = new HG::Rendering::Base::CubeMapTexture(
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/right.png"),
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/left.png"),
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/top.png"),
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/bottom.png"),
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/front.png"),
//        application.resourceManager()
//            ->load<HG::Utils::STBImageLoader>("Assets/Skybox/back.png")
//    );
//
//    application.renderer()->setup(cubeMap);
//
//    return cubeMap;
//}

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    InfoF() << "Creating application";
    HG::Core::Application application(argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()
            ->setResourceAccessor<HG::Standard::FilesystemResourceAccessor>();

    // Setting system controller
    application.setSystemController<HG::Rendering::OpenGL::GLFWSystemController>();

    // Setting rendering to forward
    application.renderer()
        ->setPipeline<HG::Rendering::OpenGL::Forward::RenderingPipeline>()
        ->addRenderer(new HG::Rendering::OpenGL::Forward::MeshRenderer)
        ->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::MeshDataProcessor)
        ->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor)
        ->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);

    if (!application.init())
    {
        ErrorF() << "Can't init application.";
        return -1;
    }

    application.setScene(new TestScene());

    return application.exec();
}