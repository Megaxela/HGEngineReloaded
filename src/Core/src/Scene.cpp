#include <GameObjectCache.hpp>
#include <CurrentLogger.hpp>
#include "Scene.hpp"
#include "Renderer.hpp"

CORE_MODULE_NS::Scene::Scene() :
    m_mainApplication(nullptr),
    m_gameObjects()
{

}

void CORE_MODULE_NS::Scene::setApplication(CORE_MODULE_NS::Application *application)
{
    m_mainApplication = application;
}

CORE_MODULE_NS::Application *CORE_MODULE_NS::Scene::application() const
{
    return m_mainApplication;
}

void CORE_MODULE_NS::Scene::update()
{
    m_gameObjects.merge();

    for (auto&& gameObject : m_gameObjects)
    {
        gameObject->update();
    }
}

void CORE_MODULE_NS::Scene::render(RENDERING_BASE_MODULE_NS::Renderer* renderer)
{
    if (renderer == nullptr)
    {
        Error() << "No renderer is set.";
        return;
    }

    renderer->render(m_gameObjects);
}

void CORE_MODULE_NS::Scene::start()
{

}

void CORE_MODULE_NS::Scene::removeGameObject(CORE_MODULE_NS::GameObject* gameObject)
{
    // Removing current parent scene.
    gameObject->setParentScene(nullptr);
    m_gameObjects.remove(gameObject);

    GameObjectCache::i().cache(gameObject);
}

void CORE_MODULE_NS::Scene::addGameObject(CORE_MODULE_NS::GameObject* gameObject)
{
    // Adding current scene as parent.
    gameObject->setParentScene(this);
    m_gameObjects.add(gameObject);
}

CORE_MODULE_NS::GameObject *CORE_MODULE_NS::Scene::findGameObject(const std::string &name)
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

void CORE_MODULE_NS::Scene::findGameObjects(const std::string &name,
                                            std::vector<CORE_MODULE_NS::GameObject *> &container)
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
