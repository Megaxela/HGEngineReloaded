#include <Forward/RenderingPipeline.hpp>
#include <Forward/AbstractRenderer.hpp>
#include <SystemController.hpp>
#include <GameObject.hpp>
#include <Camera.hpp>
#include <imgui.h>


HG::Rendering::OpenGL::Forward::RenderingPipeline::RenderingPipeline(HG::Core::Application* application) :
    HG::Rendering::Base::RenderingPipeline(application),
    m_behavioursCache(),
    m_sortedBehaviours(),
    m_gizmosRenderer(application),
    m_imguiRenderer(application),
    m_renderers()
{

}

HG::Rendering::OpenGL::Forward::RenderingPipeline::~RenderingPipeline()
{

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

    m_gizmosRenderer.init();

    m_imguiRenderer.init();

    return true;
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::deinit()
{
    HG::Rendering::Base::RenderingPipeline::deinit();

    for (auto&& [id, renderer] : m_renderers)
    {
        renderer->deinit();
    }

    m_gizmosRenderer.deinit();

    m_imguiRenderer.deinit();

    application()->renderer()->materialCollection()->clearCache();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::render(const HG::Core::Scene::GameObjectsContainer& objects)
{

    // Clearing main buffer
    gl::set_clear_color({0.0f, 0.0f, 0.0f, 1.0f});

    gl::clear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    // Getting camera
    auto camera = HG::Rendering::Base::Camera::active();

    if (camera != nullptr)
    {
        proceedGameObjects(objects);
    }

    // Render Gizmos
    application()->renderer()->gizmos()->visitShapes(m_gizmosRenderer);

    m_gizmosRenderer.render();

    // Render ImGui
    ImGui::Render();

    m_imguiRenderer.render();

    // Swapping graphics buffers
    application()->systemController()->swapBuffers();
}

void HG::Rendering::OpenGL::Forward::RenderingPipeline::proceedGameObjects(const HG::Core::Scene::GameObjectsContainer& objects)
{
    // todo: Add cubemap rendering

    m_sortedBehaviours.clear();

    // Getting camera positions
    glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
    glm::quat cameraRot;

    auto* camera = HG::Rendering::Base::Camera::active();

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

            // Not inverting, because Z is positive towards camera)
            m_sortedBehaviours.insert({
                (cameraSpace * glm::inverse(cameraRot)).z,
                behaviour
            });
        }
    }

    for (auto& [distance, behaviour] : m_sortedBehaviours)
    {
        auto renderer = m_renderers.find(behaviour->renderBehaviourType());

        if (renderer == m_renderers.end())
        {
            Info()
                << "Trying to render unknown render behaviour \""
                << SystemTools::getTypeName(behaviour)
                << "\"";
            continue;
        }

        renderer->second->render(behaviour);
    }
}