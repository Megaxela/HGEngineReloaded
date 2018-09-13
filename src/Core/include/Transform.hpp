#pragma once

// C++ STL
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace HG::Core
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
         * @brief Default constructor.
         */
        Transform();

        /**
         * @brief Constructor.
         */
        explicit Transform(GameObject* owner);

        /**
         * @brief Copy constructor. On copy - parent will not be copied.
         * Local position will be set with global position value.
         * @param transform RHS transform.
         */
        Transform(const Transform& transform);

        /**
         * @brief Copy operator. On copy - parent will not be copied.
         * Local position will be set with global position value.
         * @param transform RHS transform.
         * @return Reference to this object. (STD)
         */
        Transform& operator=(const Transform& transform);

        /**
         * @brief Destructor.
         */
        ~Transform();

        /**
         * @brief Method to rotate transform information
         * @param anchor Rotation anchor.
         * @param rotValue Rotation value.
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
         * @brief Method for getting global object scale.
         * @return Global object Vector3 scale.
         */
        glm::vec3 globalScale() const;

        /**
         * @brief Method for setting global object scale.
         * @param scale Global object scale.
         */
        void setGlobalScale(const glm::vec3& scale);

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
         * @brief Method for getting global object rotation.
         * @return Local object Quaternion rotation.
         */
        glm::quat globalRotation() const;

        /**
         * @brief Method for setting global object rotation.
         * @param rotation Global object rotation.
         */
        void setGlobalRotation(const glm::quat& rotation);

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
         * @brief Method for taking model local to world matrix.
         * @return 4x4 matrix.
         */
        glm::mat4 localToWorldMatrix() const;

        /**
         * @brief Method for setting translation, rotation and scale
         * from local to world matrix.
         * @param matrix Matrix.
         */
        void setFromLocalToWorldMatrix(const glm::mat4& matrix);

        /**
         * @brief Method for getting transform children.
         * @return Vector with children.
         */
        const std::vector<Transform*>& children() const;

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

