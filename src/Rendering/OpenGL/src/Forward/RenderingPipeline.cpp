// HG::Core
#include <GameObject.hpp>

// HG::Rendering::OpenGL
#include <Forward/RenderingPipeline.hpp>
#include <Forward/AbstractRenderer.hpp>
#include <GizmosRenderer.hpp>
#include <ImGuiRenderer.hpp>

// HG::Rendering::Base
#include <MaterialCollection.hpp>
#include <SystemController.hpp>
#include <RenderBehaviour.hpp>
#include <RenderTarget.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>

// ImGui
#include <imgui.h>
#include <Common/RenderTargetData.hpp>
#include <Behaviours/CubeMap.hpp>

HG::Rendering::OpenGL::Forward::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    HG::Rendering::Base::RenderingPipeline(application),
    m_behavioursCache(),
    m_sortedBehaviours(),
    m_gizmosRenderer(new HG::Rendering::OpenGL::GizmosRenderer(application)),
    m_imguiRenderer(new HG::Rendering::OpenGL::ImGuiRenderer(application)),
    m_renderers()
{

}

HG::Rendering::OpenGL::Forward::RenderingPipeline::~RenderingPipeline()
{
    for (auto&& renderer : m_renderers)
    {
        delete renderer.second;
    }

    delete m_gizmosRenderer;
    delete m_imguiRenderer;
}

HG::Rendering::OpenGL::Forward::RenderingPipeline *HG::Rendering::OpenGL::Forward::RenderingPipeline::addRenderer(HG::Rendering::OpenGL::Forward::AbstractRenderer *renderer)
{
    renderer->setApplication(application());
    m_renderers[renderer->getTarget()] = renderer;

    return this;
}

bool HG::Rendering::OpenGL::Forward::RenderingPipeline::init()
{
    if (!HG::Rendering::Base::RenderingPipeline::init())
    {
        return false;
    }

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->init();
    }

    m_gizmosRenderer->init();

    m_imguiRenderer->init();

    return true;
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::deinit()
{
    HG::Rendering::Base::RenderingPipeline::deinit();

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->deinit();
    }

    m_gizmosRenderer->deinit();

    m_imguiRenderer->deinit();

    application()->renderer()->materialCollection()->clearCache();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::clear(HG::Utils::Color color)
{
    updateViewport();

    // Clearing main buffer
    auto colorArray = color.toRGBAVector().data;

    gl::set_clear_color({
        colorArray.data[0],
        colorArray.data[1],
        colorArray.data[2],
        colorArray.data[3]
    });

    gl::clear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::render(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects)
{
    clear(HG::Utils::Color::fromRGB(25, 25, 25));

    // Getting camera
    auto camera = application()->renderer()->activeCamera();

    if (camera != nullptr)
    {
        proceedGameObjects(objects);
    }

    // Render Gizmos
    application()->renderer()->gizmos()->visitShapes(*m_gizmosRenderer);
    m_gizmosRenderer->render();

    // Render ImGui
    ImGui::Render();
    m_imguiRenderer->render();

    // Swapping graphics buffers
    application()->systemController()->swapBuffers();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::proceedGameObjects(const HG::Utils::DoubleBufferContainer<HG::Core::GameObject*>& objects)
{
    // todo: Add cubemap rendering

    m_sortedBehaviours.clear();
    HG::Rendering::Base::RenderBehaviour* cubemapBehaviour = nullptr;

    // Getting camera positions
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::quat cameraRot;

    auto* camera = application()->renderer()->activeCamera();

    cameraPos = camera->gameObject()->transform()->globalPosition();
    cameraRot = camera->gameObject()->transform()->globalRotation();

    // Using multimap for sorting objects by distance from camera
    for (auto&& gameObject : objects)
    {
        if (!gameObject->isEnabled())
        {
            continue;
        }

        m_behavioursCache.clear();

        // Getting rendering behaviours of GO
        gameObject->getRenderingBehaviours(m_behavioursCache);

        for (auto&& behaviour : m_behavioursCache)
        {
            if (!behaviour->isEnabled())
            {
                continue;
            }

            auto cameraSpace = gameObject->transform()->globalPosition() - cameraPos;

            if (behaviour->renderBehaviourType() == HG::Rendering::Base::Behaviours::CubeMap::RenderBehaviourId)
            {
                if (cubemapBehaviour)
                {
                    Warning() << "Several cubemap behaviours are available at the same time.";
                }

                cubemapBehaviour = behaviour;
            }
            else
            {
                // Not inverting, because Z is positive towards camera)
                m_sortedBehaviours.insert({
                    (cameraSpace * glm::inverse(cameraRot)).z,
                    behaviour
                });
            }
        }
    }

    if (cubemapBehaviour != nullptr)
    {
        // Disabling depth test
        gl::set_depth_test_enabled(false);

        // Render cubemap
        render(cubemapBehaviour);

        // Enabling depth test back
        gl::set_depth_test_enabled(true);
    }

    // Rendering other scene scene
    for (auto& [distance, behaviour] : m_sortedBehaviours)
    {
        render(behaviour);
    }
}

bool HG::Rendering::OpenGL::Forward::RenderingPipeline::render(HG::Rendering::Base::RenderBehaviour* behaviour)
{
    auto rendererIterator = m_renderers.find(behaviour->renderBehaviourType());

    if (rendererIterator == m_renderers.end())
    {
        Info()
            << "Trying to render unknown render behaviour \""
            << SystemTools::getTypeName(behaviour)
            << "\"";
        return false;
    }

    // If rendertarget size changed - change viewport
    updateViewport();

    rendererIterator->second->render(behaviour);

    return true;
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::setRenderTarget(HG::Rendering::Base::RenderTarget* target)
{
    HG::Rendering::Base::RenderingPipeline::setRenderTarget(target);

    if (needSetup(target))
    {
        if (!setup(target))
        {
            Error() << "Can't setup target.";
            return;
        }
    }

    auto externalData = static_cast<Common::RenderTargetData*>(target->specificData());

    externalData->Framebuffer.bind(GL_FRAMEBUFFER);

    updateViewport();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::updateViewport()
{
    if (m_cachedViewport != renderTarget()->size())
    {
        m_cachedViewport = renderTarget()->size();
        gl::set_viewport({0, 0}, {m_cachedViewport.x, m_cachedViewport.y});
    }
}
