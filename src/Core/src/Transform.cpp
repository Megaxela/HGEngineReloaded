#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>
#include "Transform.hpp"

CORE_MODULE_NS::Transform::Transform() :
    Transform(nullptr)
{

}

CORE_MODULE_NS::Transform::Transform(GameObject* owner) :
    m_localRotation(0.0f, 0.0f, 0.0f, 1.0f), // Initial
    m_localScale(1.0f, 1.0f, 1.0f),
    m_localPosition(),
    m_owner(owner),
    m_parent(nullptr),
    m_children()
{

}

CORE_MODULE_NS::Transform::~Transform()
{
    if (m_parent != nullptr)
    {
        m_parent->m_children.erase(
            std::find(
                m_parent->m_children.begin(),
                m_parent->m_children.end(),
                this
            )
        );
    }
}

void CORE_MODULE_NS::Transform::rotateAround(glm::vec3 anchor, glm::quat rotValue)
{
    // Transformations
    auto matrix = localToWorldMatrix();

    // Anchor rotation
    matrix = glm::translate(matrix, anchor );
    matrix = matrix * glm::mat4_cast(rotValue);

    matrix = glm::translate(matrix, anchor * -1.0f );

    // Taking data back
    glm::vec3 scale{};
    glm::quat rotation{};
    glm::vec3 translation{};
    glm::vec3 skew{};
    glm::vec4 perspective{};

    glm::decompose(matrix, scale, rotation, translation, skew, perspective);


    rotation = glm::conjugate(rotation);

    // Setting new values
    setLocalPosition(translation);
    setLocalRotation(rotation);
}

glm::vec3 CORE_MODULE_NS::Transform::localScale() const
{
    return m_localScale;
}

void CORE_MODULE_NS::Transform::setLocalScale(const glm::vec3& scale)
{
    m_localScale = scale;
}

glm::quat CORE_MODULE_NS::Transform::localRotation() const
{
    return m_localRotation;
}

void CORE_MODULE_NS::Transform::setLocalRotation(const glm::quat& rotation)
{
    m_localRotation = rotation;
}

glm::vec3 CORE_MODULE_NS::Transform::localPosition() const
{
    return m_localPosition;
}

void CORE_MODULE_NS::Transform::setLocalPosition(const glm::vec3& localPosition)
{
    m_localPosition = localPosition;
}

glm::vec3 CORE_MODULE_NS::Transform::globalPosition() const
{
    // Calculate
    if (m_parent == nullptr)
    {
        return m_localPosition;
    }
    else
    {
        return m_parent->globalPosition() +
               m_parent->globalRotation() *
               m_localPosition;
    }
}

void CORE_MODULE_NS::Transform::setGlobalPosition(const glm::vec3& globalPosition)
{
    if (m_parent == nullptr)
    {
        m_localPosition = globalPosition;
    }
    else
    {
        m_localPosition = (globalPosition - m_parent->globalPosition()) * glm::inverse(m_parent->globalRotation());
    }

}

glm::quat CORE_MODULE_NS::Transform::globalRotation() const
{
    if (m_parent == nullptr)
    {
        return m_localRotation;
    }

    return m_parent->globalRotation() * m_localRotation;
}

void CORE_MODULE_NS::Transform::setParent(CORE_MODULE_NS::Transform* transform)
{
    if (transform == nullptr)
    {
        auto currentGlobalPosition = globalPosition();
        auto currentGlobalRotation = globalRotation();

        if (m_parent != nullptr)
        {
            m_parent->m_children.erase(
                std::find(
                    m_parent->m_children.begin(),
                    m_parent->m_children.end(),
                    this
                )
            );
        }

        m_parent = nullptr;

        m_localPosition = currentGlobalPosition;
        m_localRotation = currentGlobalRotation;
    }
    else
    {
        auto currentGlobalPosition = (globalPosition() - transform->globalPosition()) * glm::inverse(transform->globalRotation());

        if (m_parent != nullptr)
        {
            m_parent->m_children.erase(
                std::find(
                    m_parent->m_children.begin(),
                    m_parent->m_children.end(),
                    this
                )
            );
        }

        m_parent = transform;

        transform->m_children.push_back(this);

        m_localPosition = currentGlobalPosition;
        m_localRotation = glm::inverse(transform->globalRotation()) * m_localRotation;
    }
}

CORE_MODULE_NS::Transform* CORE_MODULE_NS::Transform::parent() const
{
    return m_parent;
}

glm::mat4 CORE_MODULE_NS::Transform::localToWorldMatrix() const
{
    auto model = glm::translate(glm::mat4(1.0f), globalPosition());
    model = model * glm::mat4_cast(globalRotation());
    model = glm::scale(model, localScale());
    
    return model;
}

std::size_t CORE_MODULE_NS::Transform::numberOfChildren() const
{
    return m_children.size();
}

CORE_MODULE_NS::Transform* CORE_MODULE_NS::Transform::child(size_t index) const
{
    return m_children.at(index);
}

CORE_MODULE_NS::GameObject* CORE_MODULE_NS::Transform::gameObject() const
{
    return m_owner;
}
