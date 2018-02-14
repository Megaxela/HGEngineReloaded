#include <stdexcept>
#include "Application.hpp"
#include "Scene.hpp"

CORE_MODULE_NS::Application::Application(int /* argc */, char** /* argv */) :
    m_currentScene(nullptr),
    m_cachedScene(nullptr),
    m_working(false)
{

}

void CORE_MODULE_NS::Application::setScene(CORE_MODULE_NS::Scene* scene)
{
    if (scene == nullptr)
    {
        throw std::runtime_error("New scene can't be nullptr.");
    }

    scene->setApplication(this);
    m_cachedScene = scene;
}

CORE_MODULE_NS::Scene* CORE_MODULE_NS::Application::scene() const
{
    if (m_cachedScene != nullptr)
    {
        return m_cachedScene;
    }

    return m_currentScene;
}

void CORE_MODULE_NS::Application::stop()
{
    m_working = false;
}

int CORE_MODULE_NS::Application::exec()
{
    m_working = true;

    // Preparing deltaTime calculation

    while (m_working)
    {
        // Checking for new scene, etc
        proceedScene();

        // Calling update on scene.
        m_currentScene->update();

        // Calling render on scene
        m_currentScene->render();

        // Executing rendering pipeline.

    }

    if (m_currentScene != nullptr)
    {
        delete m_currentScene;
        m_currentScene = nullptr;
    }

    if (m_cachedScene != nullptr)
    {
        delete m_cachedScene;
        m_cachedScene = nullptr;
    }

    return 0;
}

void CORE_MODULE_NS::Application::proceedScene()
{
    if (m_cachedScene != nullptr)
    {
        // Deleting current scene
        delete m_currentScene;
        m_currentScene = m_cachedScene;
        m_cachedScene = nullptr;

        // Calling start method
        m_currentScene->start();
    }
}

RENDERING_BASE_MODULE_NS::Renderer* CORE_MODULE_NS::Application::renderer() const
{
    return &m_renderer;
}
