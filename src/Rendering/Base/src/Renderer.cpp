#include <CurrentLogger.hpp>
#include "Renderer.hpp"
#include <Application.hpp>
#include <Renderer.hpp>


RENDERING_BASE_MODULE_NS::Renderer::Renderer(::CORE_MODULE_NS::Application* application) :
    m_parentApplication(application),
    m_pipeline(nullptr),
    m_materialCollection(application->resourceManager(), this)
{
    Debug() << "Creating renderer.";
}

RENDERING_BASE_MODULE_NS::Renderer::~Renderer()
{
    delete m_pipeline;

    Debug() << "Destroying renderer.";
}

bool RENDERING_BASE_MODULE_NS::Renderer::init()
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return false;
    }

    return m_pipeline->init();
}

void RENDERING_BASE_MODULE_NS::Renderer::render(const CORE_MODULE_NS::Scene::GameObjectsContainer& gameObjects)
{
    if (m_pipeline == nullptr)
    {
        Error() << "No rendering pipeline set.";
        return;
    }

    m_pipeline->render(gameObjects);

    m_gizmos.clear();
}

::RENDERING_BASE_MODULE_NS::Gizmos *::RENDERING_BASE_MODULE_NS::Renderer::gizmos()
{
    return &m_gizmos;
}

RENDERING_BASE_MODULE_NS::RenderingPipeline* RENDERING_BASE_MODULE_NS::Renderer::pipeline()
{
    return m_pipeline;
}

::RENDERING_BASE_MODULE_NS::MaterialCollection* RENDERING_BASE_MODULE_NS::Renderer::materialCollection()
{
    return &m_materialCollection;
}
