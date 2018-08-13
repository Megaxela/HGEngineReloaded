#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>
#include <CurrentLogger.hpp>
#include <Transform.hpp>

#include "Transform.hpp"

HG::Core::Transform::Transform() :
    Transform(nullptr)
{

}

HG::Core::Transform::Transform(GameObject* owner) :
    m_localRotation(1.0f, 0.0f, 0.0f, 0.0f), // Initial
    m_localScale(1.0f, 1.0f, 1.0f),
    m_localPosition(),
    m_owner(owner),
    m_parent(nullptr),
    m_children()
{

}

HG::Core::Transform::~Transform()
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

void HG::Core::Transform::rotateAround(glm::vec3 anchor, glm::quat rotValue)
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

glm::vec3 HG::Core::Transform::localScale() const
{
    return m_localScale;
}

void HG::Core::Transform::setLocalScale(const glm::vec3& scale)
{
    m_localScale = scale;
}

glm::vec3 HG::Core::Transform::globalScale() const
{
    // Calculate
    if (m_parent == nullptr)
    {
        return m_localScale;
    }
    else
    {
        return m_parent->globalScale() * m_localScale;
    }
}

void HG::Core::Transform::setGlobalScale(const glm::vec3 &scale)
{
    if (m_parent == nullptr)
    {
        m_localScale = scale;
    }
    else
    {
        m_localScale = (scale / m_parent->globalScale());
    }
}

glm::quat HG::Core::Transform::localRotation() const
{
    return m_localRotation;
}

void HG::Core::Transform::setLocalRotation(const glm::quat& rotation)
{
    m_localRotation = rotation;
}

glm::vec3 HG::Core::Transform::localPosition() const
{
    return m_localPosition;
}

void HG::Core::Transform::setLocalPosition(const glm::vec3& localPosition)
{
    m_localPosition = localPosition;
}

glm::vec3 HG::Core::Transform::globalPosition() const
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

void HG::Core::Transform::setGlobalPosition(const glm::vec3& globalPosition)
{
    if (m_parent == nullptr)
    {
        m_localPosition = globalPosition;
    }
    else
    {
        m_localPosition =
            (globalPosition - m_parent->globalPosition()) *
             glm::inverse(m_parent->globalRotation());
    }

}

glm::quat HG::Core::Transform::globalRotation() const
{
    if (m_parent == nullptr)
    {
        return m_localRotation;
    }

    return m_parent->globalRotation() * m_localRotation;
}

void HG::Core::Transform::setParent(HG::Core::Transform* transform)
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

HG::Core::Transform* HG::Core::Transform::parent() const
{
    return m_parent;
}

glm::mat4 HG::Core::Transform::localToWorldMatrix() const
{
    auto model = glm::translate(glm::mat4(1.0f), globalPosition());
    model = model * glm::mat4_cast(globalRotation());
    model = glm::scale(model, globalScale());
    
    return model;
}

void HG::Core::Transform::setFromLocalToWorldMatrix(const glm::mat4& matrix)
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(
        matrix,
        scale,
        rotation,
        position,
        skew,
        perspective
    );

    setGlobalPosition(position);
    setLocalRotation(rotation);
    setGlobalScale(scale);
}

const std::vector<HG::Core::Transform *> &HG::Core::Transform::children() const
{
    return m_children;
}

HG::Core::GameObject* HG::Core::Transform::gameObject() const
{
    return m_owner;
}

HG::Core::Transform::Transform(const HG::Core::Transform &transform) :
    Transform()
{
    (*this) = transform;
}

HG::Core::Transform &HG::Core::Transform::operator=(const HG::Core::Transform &transform)
{
    // todo: Recalculate local rotation from current parent's rotation
    m_localRotation = transform.globalRotation();

    // todo: If global scale will appear, maybe global info must be copied here
    setGlobalScale(transform.globalScale());
    setGlobalPosition(transform.globalPosition());

    return *this;
}
