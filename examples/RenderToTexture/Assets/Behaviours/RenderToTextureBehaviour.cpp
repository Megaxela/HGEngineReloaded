// RenderToTexture
#include <Assets/Behaviours/RenderToTextureBehaviour.hpp>

// HG::Core
#include <Scene.hpp>
#include <Application.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>
#include <Material.hpp>
#include <RenderingPipeline.hpp>
#include <RenderTarget.hpp>
#include <Behaviours/Mesh.hpp>


RenderToTextureBehaviour::RenderToTextureBehaviour() :
    m_renderTarget(new HG::Rendering::Base::RenderTarget({200, 200})),
    m_renderBehaviour(nullptr)
{

}

RenderToTextureBehaviour::~RenderToTextureBehaviour()
{
    delete m_renderTarget;
}

void RenderToTextureBehaviour::setRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    m_renderBehaviour = behaviour;
}

void RenderToTextureBehaviour::setTarget(HG::Rendering::Base::Texture* texture)
{
    m_renderTarget->setColorTexture(texture, 0);
}

void RenderToTextureBehaviour::onUpdate()
{
    if (!m_renderBehaviour)
    {
        return;
    }

    auto pipeline = scene()->application()->renderer()->pipeline();

    auto previous = pipeline->renderTarget();
    {
        pipeline->setRenderTarget(m_renderTarget);

        pipeline->clear(HG::Utils::Color::Red);
        pipeline->render(m_renderBehaviour);
    }
    pipeline->setRenderTarget(previous);
}
