#include <stdexcept>
#include "Application.hpp"
#include "Scene.hpp"

HG::Core::Application::Application(int /* argc */, char** /* argv */) :
    m_currentScene(nullptr),
    m_cachedScene(nullptr),
    m_working(false)
{

}

void HG::Core::Application::setScene(HG::Core::Scene* scene)
{
    if (scene == nullptr)
    {
        throw std::runtime_error("New scene can't be nullptr.");
    }

    scene->setApplication(this);
    m_cachedScene = scene;
}

HG::Core::Scene* HG::Core::Application::scene() const
{
    if (m_cachedScene != nullptr)
    {
        return m_cachedScene;
    }

    return m_currentScene;
}

void HG::Core::Application::stop()
{
    m_working = false;
}

int HG::Core::Application::exec()
{
    m_working = true;

    // Preparing deltaTime calculation

    while (m_working)
    {
        // Checking for new scene, etc
        proceedScene();

        // Calling update on scenes.
        m_currentScene->update();
    }

    return 0;
}

void HG::Core::Application::proceedScene()
{
    if (m_cachedScene != nullptr)
    {
        // Deleting current scene
        delete m_currentScene;
        m_currentScene = m_cachedScene;
        m_cachedScene = nullptr;

        // Calling start method

    }
}
