#include <Scene.hpp>
#include "ForwardRenderingPipeline.hpp"
#include <Application.hpp>

OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::ForwardRenderingPipeline(::CORE_MODULE_NS::Application* application) :
    RenderingPipeline(application)
{

}

void OGL_RENDERING_MODULE_NS::ForwardRenderingPipeline::render(const HG::Core::Scene::GameObjectsContainer& objects)
{
    application()->systemController()->swapBuffers();
}
