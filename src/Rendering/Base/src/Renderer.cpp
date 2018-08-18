#include <CurrentLogger.hpp>
#include "Renderer.hpp"
#include <Application.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>


HG::Rendering::Base::Renderer::Renderer(HG::Core::Application* application) :
    m_parentApplication(application),
    m_pipeline(nullptr),
    m_gizmos(),
    m_materialCollection(application->resourceManager(), this),
    m_activeCamera(nullptr)
{
    Debug() << "Creating renderer.";
}

HG::Rendering::Base::Renderer::~Renderer()
{
    delete m_pipeline;

    Debug() << "Destroying renderer.";
}

bool HG::Rendering::Base::Renderer::init()
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return false;
    }

    return m_pipeline->init();
}

void HG::Rendering::Base::Renderer::deinit()
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    return m_pipeline->deinit();
}

void HG::Rendering::Base::Renderer::render(const HG::Core::Scene::GameObjectsContainer& gameObjects)
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    m_pipeline->render(gameObjects);

    m_gizmos.clear();
}

HG::Rendering::Base::Gizmos *HG::Rendering::Base::Renderer::gizmos()
{
    return &m_gizmos;
}

HG::Rendering::Base::RenderingPipeline* HG::Rendering::Base::Renderer::pipeline()
{
    return m_pipeline;
}

HG::Rendering::Base::MaterialCollection* HG::Rendering::Base::Renderer::materialCollection()
{
    return &m_materialCollection;
}

HG::Rendering::Base::Camera *HG::Rendering::Base::Renderer::activeCamera() const
{
    return m_activeCamera;
}

void HG::Rendering::Base::Renderer::setActiveCamera(HG::Rendering::Base::Camera *camera)
{
    if (camera->gameObject() == nullptr)
    {
        return;
    }

    m_activeCamera = camera;
}
