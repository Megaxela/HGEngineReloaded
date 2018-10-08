// HG::Core
#include <GameObject.hpp>
#include <GameObjectCache.hpp>
#include <Scene.hpp>

// HG::Rendering::Base
#include <Renderer.hpp>

// ALogger
#include <CurrentLogger.hpp>

HG::Core::Scene::Scene() :
    m_mainApplication(nullptr),
    m_gameObjects()
{

}

HG::Core::Scene::~Scene()
{
    // Removing gameobject (caching)
    for (auto iter = m_gameObjects.addedBegin(),
              end = m_gameObjects.addedEnd();
              iter != end;
              ++iter)
    {
        removeGameObject(*iter);
    }

    for (auto&& gameObject : m_gameObjects)
    {
        removeGameObject(gameObject);
    }

    // Clearing registered resources
    for (auto&& deleter : m_deleteExecutors)
    {
        deleter();
    }
}

void HG::Core::Scene::setApplication(HG::Core::Application *application)
{
    m_mainApplication = application;
}

HG::Core::Application *HG::Core::Scene::application() const
{
    return m_mainApplication;
}

void HG::Core::Scene::update()
{
    m_gameObjects.merge();

    for (auto&& gameObject : m_gameObjects)
    {
        if (!gameObject->isEnabled())
        {
            continue;
        }

        gameObject->update();
    }
}

void HG::Core::Scene::render(HG::Rendering::Base::Renderer* renderer)
{
    if (renderer == nullptr)
    {
        Error() << "No renderer is set.";
        return;
    }

    renderer->render(m_gameObjects);
}

void HG::Core::Scene::start()
{

}

void HG::Core::Scene::removeGameObject(HG::Core::GameObject* gameObject)
{
    // Removing current parent scene.
    gameObject->setParentScene(nullptr);
    m_gameObjects.remove(gameObject);

    GameObjectCache::i().cache(gameObject);
}

void HG::Core::Scene::addGameObject(HG::Core::GameObject* gameObject)
{
    // Adding current scene as parent.
    gameObject->setParentScene(this);
    m_gameObjects.add(gameObject);
}

HG::Core::GameObject *HG::Core::Scene::findGameObject(const std::string &name)
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (m_gameObjects.isRemoving(gameObject))
        {
            continue;
        }

        if (gameObject->name() == name)
        {
            return gameObject;
        }
    }

    return nullptr;
}

void HG::Core::Scene::findGameObjects(const std::string &name,
                                            std::vector<HG::Core::GameObject *> &container)
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (m_gameObjects.isRemoving(gameObject))
        {
            continue;
        }

        if (gameObject->name() == name)
        {
            container.push_back(gameObject);
        }
    }
}
