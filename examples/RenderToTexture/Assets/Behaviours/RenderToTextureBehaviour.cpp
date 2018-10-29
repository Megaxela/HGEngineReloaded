// RenderToTexture
#include <Assets/Behaviours/RenderToTextureBehaviour.hpp>

// HG::Core
#include <HG/Core/Scene.hpp>
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <Behaviours/Mesh.hpp>


RenderToTextureBehaviour::RenderToTextureBehaviour() :
    m_renderTarget1(new HG::Rendering::Base::RenderTarget({800, 800})),
    m_renderTarget2(new HG::Rendering::Base::RenderTarget({800, 800})),
    m_renderBehaviour(nullptr),
    m_switch(false)
{

}

RenderToTextureBehaviour::~RenderToTextureBehaviour()
{
    delete m_renderTarget1;
    delete m_renderTarget2;
}

void RenderToTextureBehaviour::setRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    m_renderBehaviour = behaviour;
}

void RenderToTextureBehaviour::setTarget1(HG::Rendering::Base::Texture* texture)
{
    m_renderTarget1->setColorTexture(texture, 0);
}

void RenderToTextureBehaviour::setTarget2(HG::Rendering::Base::Texture* texture)
{
    m_renderTarget2->setColorTexture(texture, 0);
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
        pipeline->setRenderTarget(m_switch ? m_renderTarget2 : m_renderTarget1);

        auto rb = static_cast<HG::Rendering::Base::Behaviours::Mesh*>(m_renderBehaviour);

        pipeline->clear(HG::Utils::Color::Red);

        rb->material()->set(
            "textureMap",
            m_switch
            ?
            m_renderTarget1->colorTexture(0)
            :
            m_renderTarget2->colorTexture(0)
        );
        pipeline->render(m_renderBehaviour);
    }
    pipeline->setRenderTarget(previous);

    m_switch = !m_switch;
}
