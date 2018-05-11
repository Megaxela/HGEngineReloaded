#include <stdexcept>
#include "Application.hpp"
#include "Scene.hpp"

CORE_MODULE_NS::Application::Application(int /* argc */, char** /* argv */) :
    m_renderer(this),
    m_systemController(nullptr),
    m_resourceManager(),
    m_currentScene(nullptr),
    m_cachedScene(nullptr)
{

}

CORE_MODULE_NS::Application::~Application()
{
    delete m_systemController;
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
    const_cast<Input::Window*>(m_input.window())->setClosed(true);
}

bool CORE_MODULE_NS::Application::init()
{
    return m_renderer.init();
}

bool CORE_MODULE_NS::Application::performCycle()
{
    // Start counting frame time
    m_timeStatistics.tickTimerBegin(TimeStatistics::FrameTime);

    // Ticking pushed/released values in input subsystem
    m_input.tickControllers();

    // Polling events
    if (m_renderer.pipeline() != nullptr &&
        systemController() != nullptr)
    {
        systemController()->pollEvents();
    }

    // Checking for new scene, etc
    proceedScene();

    {
        // Start counting update time
        m_timeStatistics.tickTimerBegin(TimeStatistics::UpdateTime);

        // Calling update on scene.
        m_currentScene->update();

        // Finishing counting update time
        m_timeStatistics.tickTimerEnd(TimeStatistics::UpdateTime);
    }

    {
        // Start counting rendering time
        m_timeStatistics.tickTimerBegin(TimeStatistics::RenderTime);

        // Executing rendering pipeline.
        m_currentScene->render(&m_renderer);

        // Finishing counting rendering time
        m_timeStatistics.tickTimerEnd(TimeStatistics::RenderTime);
    }

    // Finishing counting frame time
    m_timeStatistics.tickTimerEnd(TimeStatistics::FrameTime);

    return true;
}

int CORE_MODULE_NS::Application::exec()
{
    // Preparing deltaTime calculation

    while (!m_input.window()->isClosed())
    {
        performCycle();
    }

    delete m_currentScene;
    m_currentScene = nullptr;

    delete m_cachedScene;
    m_cachedScene = nullptr;

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

RENDERING_BASE_MODULE_NS::Renderer* CORE_MODULE_NS::Application::renderer()
{
    return &m_renderer;
}

CORE_MODULE_NS::ResourceManager* CORE_MODULE_NS::Application::resourceManager()
{
    return &m_resourceManager;
}

CORE_MODULE_NS::TimeStatistics* CORE_MODULE_NS::Application::timeStatistics()
{
    return &m_timeStatistics;
}

const CORE_MODULE_NS::Input *CORE_MODULE_NS::Application::input() const
{
    return &m_input;
}

::RENDERING_BASE_MODULE_NS::SystemController *CORE_MODULE_NS::Application::systemController() const
{
    return m_systemController;
}
