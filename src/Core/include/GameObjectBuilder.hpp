#pragma once

#include "GameObject.hpp"

namespace CORE_MODULE_NS
{
    /**
     * @brief Singleton builder
     * for game objects.
     *
     * Sample usage:
     * ```cpp
     * GameObject* go = HG::Core::GameObjectBuilder()
     *                      .setName("SomeName")
     *                      .setPosition(glm::vec3(1, 2, 3))
     *                      .addBehaviour(new SomeBehaviour);
     * ```
     */
    class GameObjectBuilder
    {
    public:

        /**
         * @brief Constructor.
         */
        GameObjectBuilder();

        // Disable copying
        GameObjectBuilder(const GameObjectBuilder&) = delete;
        GameObjectBuilder& operator=(const GameObjectBuilder&) = delete;

        /**
         * @brief Non explicit cast from builder
         * to gameobject. (Deploying)
         * @return Pointer to created game object.
         */
        operator GameObject*();

        /**
         * @brief Method for setting game object to setup.
         * @param ptr GameObject pointer.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setGameObject(GameObject* ptr);


        /**
         * @brief Method for adding behaviour to
         * game object.
         * @param behaviour Pointer to behaviour.
         * @return Reference to builder.
         */
        GameObjectBuilder& addBehaviour(Behaviour* behaviour);

        /**
         * @brief Method for adding rendering behaviour
         * to game object.
         * @param renderingBehaviour Pointer to rendering behaviour.
         * @return Reference to builder.
         */
        GameObjectBuilder& addRenderingBehaviour(RENDERING_BASE_MODULE_NS::RenderBehaviour* renderingBehaviour);

        /**
         * @brief Method to set game object local position.
         * @param position Vector3 position value.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setLocalPosition(const glm::vec3 &position);

        /**
         * @brief Method to set game obejct global position
         * @param position Vector3 position value.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setGlobalPosition(const glm::vec3 &position);

        /**
         * @brief Method to set game object rotation.
         * @param rotation Quaternion rotation value.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setRotation(const glm::quat &rotation);

        /**
         * @brief Method to set game object scale.
         * @param scale Vector3 scale value.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setScale(const glm::vec3 &scale);

        /**
         * @brief Method to set game object name.
         * @param name ASCII name.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setName(std::string name);

        /**
         * @brief Method to set game object parent.
         * @param parent Parent GameObject.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setParent(GameObject* parent);

        /**
         * @brief Method to set game object parent.
         * @param parent Parent game object's transform.
         * @return Reference to builder instance.
         */
        GameObjectBuilder& setParent(Transform* parent);

    private:

        /**
         * @brief Method for picking gameobject
         * as current.
         */
        void pickCurrentGameObject();

        GameObject* m_currentGameObject;
    };
}


