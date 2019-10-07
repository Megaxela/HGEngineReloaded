#pragma once

// C++ STL
#include <string>

// GLM
#include <glm/glm.hpp>

namespace HG::Core
{
class GameObject;
class Behaviour;
class Transform;
class ResourceCache;

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
    explicit GameObjectBuilder(HG::Core::ResourceCache* cache);

    // Disable copying
    GameObjectBuilder(const GameObjectBuilder&) = delete;
    GameObjectBuilder& operator=(const GameObjectBuilder&) = delete;

    /**
     * @brief Non explicit cast from builder
     * to gameobject. (Deploying)
     * @return Pointer to created game object.
     */
    operator HG::Core::GameObject*();

    /**
     * @brief Explicit deployment. Maybe useful
     * if required to save in auto variable.
     * @return
     */
    HG::Core::GameObject* deploy();

    /**
     * @brief Method for setting game object hidden
     * state.
     * @param hidden Hidden state.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setHidden(bool hidden);

    /**
     * @brief Method for setting game object enabled
     * state.
     * @param enabled Enabled state.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setEnabled(bool enabled);

    /**
     * @brief Method for setting game object to setup.
     * @param ptr GameObject pointer.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setGameObject(HG::Core::GameObject* ptr);

    /**
     * @brief Method for adding behaviour to
     * game object.
     * @param behaviour Pointer to behaviour.
     * @return Reference to builder.
     */
    GameObjectBuilder& addBehaviour(HG::Core::Behaviour* behaviour);

    /**
     * @brief Method to set game object local position.
     * @param position Vector3 position value.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setLocalPosition(const glm::vec3& position);

    /**
     * @brief Method to set game obejct global position
     * @param position Vector3 position value.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setGlobalPosition(const glm::vec3& position);

    /**
     * @brief Method to set game object rotation.
     * @param rotation Quaternion rotation value.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setRotation(const glm::quat& rotation);

    /**
     * @brief Method to set game object scale.
     * @param scale Vector3 scale value.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setScale(const glm::vec3& scale);

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
    GameObjectBuilder& setParent(HG::Core::GameObject* parent);

    /**
     * @brief Method to set game object parent.
     * @param parent Parent game object's transform.
     * @return Reference to builder instance.
     */
    GameObjectBuilder& setParent(HG::Core::Transform* parent);

private:
    HG::Core::GameObject* m_currentGameObject;
};
} // namespace HG::Core
