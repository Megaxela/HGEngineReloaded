// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Scene.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Core
{
Scene::Scene() : m_mainApplication(nullptr), m_gameObjects()
{
}

Scene::~Scene()
{
    // Removing gameobject (caching)
    for (auto&& behaviour : m_gameObjects.added())
    {
        removeGameObject(behaviour);
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

void Scene::setApplication(Application* application)
{
    m_mainApplication = application;
}

Application* Scene::application() const
{
    return m_mainApplication;
}

void Scene::update()
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

void Scene::render(HG::Rendering::Base::Renderer* renderer)
{
    if (renderer == nullptr)
    {
        HGError() << "No renderer is set.";
        return;
    }

    renderer->render(m_gameObjects);
}

void Scene::start()
{
}

void Scene::removeGameObject(GameObject* gameObject)
{
    // Removing current parent scene.
    gameObject->setParentScene(nullptr);

    delete gameObject;
    m_gameObjects.remove(gameObject);
}

void Scene::addGameObject(GameObject* gameObject)
{
    // Adding current scene as parent.
    gameObject->setParentScene(this);
    m_gameObjects.add(gameObject);
}

GameObject* Scene::findGameObject(const std::string& name) const
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->isHidden() || m_gameObjects.isRemoving(gameObject))
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

void Scene::findGameObjects(const std::string& name, std::vector<GameObject*>& container) const
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->isHidden() || m_gameObjects.isRemoving(gameObject))
        {
            continue;
        }

        if (gameObject->name() == name)
        {
            container.push_back(gameObject);
        }
    }
}

void Scene::getGameObjects(std::vector<GameObject*>& container) const
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->isHidden() || m_gameObjects.isRemoving(gameObject))
        {
            continue;
        }

        container.push_back(gameObject);
    }
}
} // namespace HG::Core
