#pragma once

// C++ STL
#include <chrono>
#include <functional>

// HG::Utils
#include <HG/Utils/DoubleBufferContainer.hpp>

namespace HG::Rendering::Base
{
    class Renderer;
}

namespace HG::Core
{
    // Forward definition
    class Application;
    class GameObject;

    /**
     * @brief Class, that describes
     * application scene. This class also
     * provides simple garbage collection system.
     * If some resource has to be deallocated (delete)
     * on scene deallocation - you have to register
     * resource with HG::Core::Scene::registerResource
     * method. You can't unregister resource from
     * this GC. Or call garbage collection at any time.
     */
    class Scene
    {
    public:

        using GameObjectsContainer = HG::Utils::DoubleBufferContainer<GameObject*>;

        /**
         * @brief Constructor.
         */
        Scene();

        // Disable copying
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        /**
         * @brief Destructor.
         */
        virtual ~Scene();

        /**
         * @brief Method for setting application
         * instance.
         * @param application Pointer to application.
         */
        void setApplication(Application* application);

        /**
         * @brief Method for getting current application
         * instance.
         * @return Pointer to application.
         */
        HG::Core::Application* application() const;

        /**
         * @brief Method, that's called every frame.
         */
        void update();

        /**
         * @brief Method, that's called every frame,
         * after `update` method.
         */
        void render(HG::Rendering::Base::Renderer* renderer);

        /**
         * @brief Method, that's called on scene start.
         */
        virtual void start();

        /**
         * @brief Method for removing gameobject
         * from scene.
         * @param gameObject GameObject.
         */
        void removeGameObject(HG::Core::GameObject* gameObject);

        /**
         * @brief Method for adding new gameObject.
         * If gameObject will be `nullptr`,
         * `std::runtime_error` exception will
         * be thrown.
         * @param gameObject Pointer to GameObject.
         */
        void addGameObject(HG::Core::GameObject* gameObject);

        /**
         * @brief Method for searching for gameobject by name.
         * @param name Gameobject name.
         * @return Pointer to found game object or nullptr if
         * game object was not found.
         */
        HG::Core::GameObject* findGameObject(const std::string& name);

        /**
         * @brief Method for searching several objects by name.
         * @param name Game Object name.
         * @param container Container for results.
         */
        void findGameObjects(const std::string& name, std::vector<HG::Core::GameObject*>& container);

        /**
         * @brief Method for getting all active gameobjects.
         * @param container Container object.
         */
        void getGameObjects(std::vector<GameObject*>& container);

        /**
         * @brief Method for registrating resource
         * for deleting after scene delete.
         * @tparam Type Type of resource.
         * @param resource Pointer to resource.
         * @return Pointer to resource.
         */
        template<typename Type>
        Type* registerResource(Type* resource)
        {
            m_deleteExecutors.push_back(
                [resource]()
                {
                    delete resource;
                }
            );

            return resource;
        }

    private:
        HG::Core::Application* m_mainApplication;
        GameObjectsContainer m_gameObjects;
        std::vector<std::function<void()>> m_deleteExecutors;
    };
}

