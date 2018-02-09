#include <GameObjectCache.hpp>
#include "Scene.hpp"

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
