#pragma once

#include <chrono>
#include <DoubleBufferContainer.hpp>

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
     * application scene.
     */
    class Scene
    {
    public:

        using GameObjectsContainer = HG::Utils::DoubleBufferContainer<GameObject*>;

        /**
         * @brief Constructor.
         */
        Scene();

        /**
         * @brief Destructor.
         */
        virtual ~Scene() = default;

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
        Application* application() const;

        /**
         * @brief Method, that's called every frame.
         */
        void update();

        /**
         * @brief Method, that's called every frame,
         * after `update` method.
         */
        void render(::HG::Rendering::Base::Renderer* renderer);

        /**
         * @brief Method, that's called on scene start.
         */
        virtual void start();

        /**
         * @brief Method for removing gameobject
         * from scene.
         * @param gameObject GameObject.
         */
        void removeGameObject(GameObject* gameObject);

        /**
         * @brief Method for adding new gameObject.
         * If gameObject will be `nullptr`,
         * `std::runtime_error` exception will
         * be thrown.
         * @param gameObject Pointer to GameObject.
         */
        void addGameObject(GameObject* gameObject);

        /**
         * @brief Method for searching for gameobject by name.
         * @param name Gameobject name.
         * @return Pointer to found game object or nullptr if
         * game object was not found.
         */
        GameObject* findGameObject(const std::string& name);

        /**
         * @brief Method for searching several objects by name.
         * @param name Game Object name.
         * @param container Container for results.
         */
        void findGameObjects(const std::string& name, std::vector<GameObject*>& container);

        /**
         * @brief Method for getting all active gameobjects.
         * @tparam Container Container type.
         * @param container Container object.
         */
        template<typename Container>
        void getGameObjects(Container& container)
        {
            for (auto&& gameObject : m_gameObjects)
            {
                if (m_gameObjects.isRemoving(gameObject))
                {
                    continue;
                }

                container.push_back(gameObject);
            }
        }

    private:
        Application* m_mainApplication;

        GameObjectsContainer m_gameObjects;
    };
}

