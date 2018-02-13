#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/quaternion.hpp>

namespace CORE_MODULE_NS
{
    class GameObject;

    /**
     * @brief Class, that describes
     * world coordinates transformation
     * with rotation, position and scale.
     */
    class Transform
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit Transform(GameObject* owner);

        /**
         * @brief Destructor.
         */
        ~Transform();

        // Disabling copying
        Transform(const Transform&) = default;
        Transform& operator=(const Transform&) = default;

        /**
         * @brief Method to rotate transform information
         * @param anchor
         * @param rotValue
         */
        void rotateAround(glm::vec3 anchor, glm::quat rotValue);

        /**
         * @brief Method for getting local object scale.
         * @return Local object Vector3 scale.
         */
        glm::vec3 localScale() const;

        /**
         * @brief Method for setting local object scale.
         * @param scale Local object Vector3 scale.
         */
        void setLocalScale(const glm::vec3 &scale);

        /**
         * @brief Method for getting local object rotation.
         * @return Local object Quaternion rotation.
         */
        glm::quat localRotation() const;

        /**
         * @brief Method for setting local object rotation.
         * @param quaternion Local object Quaternion rotation.
         */
        void setLocalRotation(const glm::quat &rotation);

        /**
         * @brief Method for getting local object position.
         * @return Local object Vector3 position.
         */
        glm::vec3 localPosition() const;

        /**
         * @brief Method for setting local object position.
         * @param localPosition Local object Vector3 position.
         */
        void setLocalPosition(const glm::vec3 &localPosition);

        /**
         * @brief Method for getting global object position.
         * @return Global object Vector3 position.
         */
        glm::vec3 globalPosition() const;

        /**
         * @brief Method for setting global object position.
         * @param globalPosition Global object Vector3 position.
         */
        void setGlobalPosition(const glm::vec3 &globalPosition);

        /**
         * @brief Method for getting global object rotation.
         * @return Local object Quaternion rotation.
         */
        glm::quat globalRotation() const;

        /**
         * @brief Method for setting parent transform object.
         * @param transform Reference to transform object.
         */
        void setParent(Transform* transform);

        /**
         * @brief Method for getting parent.
         * @return Pointer to parent transform object.
         */
        Transform* parent() const;

        /**
         * @brief Method for constructing string from transform object.
         * @return String
         */
//        std::string toString() const;

        /**
         * @brief Method for taking model local to world matrix.
         * @return 4x4 matrix.
         */
        glm::mat4 localToWorldMatrix() const;

        /**
         * @brief Method for getting number of children of this
         * transform.
         * @return Number of children.
         */
        std::size_t numberOfChildren() const;

        /**
         * @brief Method for getting child on index.
         * @param index Child index.
         * @return Pointer to child pointer or nullptr if there is
         * no such child.
         */
        Transform* child(size_t index) const;

        /**
         * @brief Method for getting gameObject of this transform.
         * @return Pointer to owner.
         */
        GameObject* gameObject() const;

    private:

        glm::quat m_localRotation;
        glm::vec3 m_localScale;
        glm::vec3 m_localPosition;
        GameObject* m_owner;
        Transform* m_parent;
        std::vector<Transform*> m_children;

    };
}

