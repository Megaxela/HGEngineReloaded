// C++ STL
#include <algorithm>

// HG::Core
#include <HG/Core/BuildProperties.hpp>
#include <HG/Core/Transform.hpp>

// GLM
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

namespace HG::Core
{
Transform::Transform() : Transform(nullptr)
{
}

Transform::Transform(GameObject* owner) :
    m_localRotation(1.0f, 0.0f, 0.0f, 0.0f), // Initial
    m_localScale(1.0f, 1.0f, 1.0f),
    m_localPosition(),
    m_owner(owner),
    m_parent(nullptr),
    m_children()
{
}

Transform::~Transform()
{
    for (auto& child : m_children)
    {
        if (child->m_parent == this)
        {
            child->m_parent = nullptr;
        }
    }

    setParent(nullptr);
}

void Transform::rotateAround(glm::vec3 anchor, glm::quat rotValue)
{
    // Transformations
    auto matrix = localToWorldMatrix();

    // Anchor rotation
    matrix = glm::translate(matrix, anchor);
    matrix = matrix * glm::mat4_cast(rotValue);

    matrix = glm::translate(matrix, anchor * -1.0f);

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

glm::vec3 Transform::localScale() const
{
    return m_localScale;
}

void Transform::setLocalScale(const glm::vec3& scale)
{
    m_localScale = scale;
}

glm::vec3 Transform::globalScale() const
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

void Transform::setGlobalScale(const glm::vec3& scale)
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

glm::quat Transform::localRotation() const
{
    return m_localRotation;
}

void Transform::setLocalRotation(const glm::quat& rotation)
{
    m_localRotation = rotation;
}

glm::vec3 Transform::localPosition() const
{
    return m_localPosition;
}

void Transform::setLocalPosition(const glm::vec3& localPosition)
{
    m_localPosition = localPosition;
}

glm::vec3 Transform::globalPosition() const
{
    // Calculate
    if (m_parent == nullptr)
    {
        return m_localPosition;
    }
    else
    {
        return m_parent->globalPosition() + m_parent->globalRotation() * m_localPosition;
    }
}

void Transform::setGlobalPosition(const glm::vec3& globalPosition)
{
    if (m_parent == nullptr)
    {
        m_localPosition = globalPosition;
    }
    else
    {
        m_localPosition = (globalPosition - m_parent->globalPosition()) * m_parent->globalRotation();
    }
}

glm::quat Transform::globalRotation() const
{
    if (m_parent == nullptr)
    {
        return m_localRotation;
    }

    return glm::normalize(m_parent->globalRotation() * m_localRotation);
}

void Transform::setGlobalRotation(const glm::quat& rotation)
{
    if (m_parent == nullptr)
    {
        m_localRotation = glm::normalize(rotation);
    }
    else
    {
        m_localRotation = glm::normalize(rotation) * glm::normalize(glm::inverse(m_parent->globalRotation()));
    }
}

void Transform::setParent(Transform* transform)
{
    if (transform == nullptr)
    {
        auto currentGlobalPosition = globalPosition();
        auto currentGlobalRotation = globalRotation();

        if (m_parent != nullptr)
        {
            if constexpr (BuildProperties::isDebug())
            {
                if (std::find(m_parent->children().begin(), m_parent->children().end(), this) ==
                    m_parent->children().end())
                {
                    throw std::runtime_error("Some corruption happened");
                }
            }

            m_parent->m_children.erase(std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this));
        }

        m_parent = nullptr;

        m_localPosition = currentGlobalPosition;
        m_localRotation = currentGlobalRotation;
    }
    else
    {
        auto currentGlobalPosition =
            (globalPosition() - transform->globalPosition()) * glm::inverse(transform->globalRotation());

        if (m_parent != nullptr)
        {
            m_parent->m_children.erase(std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this));
        }

        m_parent = transform;

        transform->m_children.push_back(this);

        m_localPosition = currentGlobalPosition;
        m_localRotation = glm::inverse(transform->globalRotation()) * m_localRotation;
    }
}

Transform* Transform::parent() const
{
    return m_parent;
}

glm::mat4 Transform::localToWorldMatrix() const
{
    auto scale = globalScale();

    // todo: Fckn monkeycoding. Fixes decomposition if scale is close to (0, 0, 0)
    if (glm::abs(scale.x) < std::numeric_limits<float>::epsilon())
    {
        scale.x = 0.001;
    }
    if (glm::abs(scale.y) < std::numeric_limits<float>::epsilon())
    {
        scale.y = 0.001;
    }
    if (glm::abs(scale.z) < std::numeric_limits<float>::epsilon())
    {
        scale.z = 0.001;
    }

    auto model = glm::translate(glm::mat4(1.0f), globalPosition());
    model      = model * glm::mat4_cast(globalRotation());
    model      = model * glm::scale(scale);

    return model;
}

void Transform::setFromLocalToWorldMatrix(const glm::mat4& matrix)
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(matrix, scale, rotation, position, skew, perspective);

    setGlobalRotation(rotation);
    setGlobalPosition(position);
    setGlobalScale(scale);
}

const std::vector<Transform*>& Transform::children() const
{
    return m_children;
}

GameObject* Transform::gameObject() const
{
    return m_owner;
}

Transform::Transform(const Transform& transform) : Transform()
{
    (*this) = transform;
}

Transform& Transform::operator=(const Transform& transform)
{
    // todo: Recalculate local rotation from current parent's rotation
    m_localRotation = transform.globalRotation();

    // todo: If global scale will appear, maybe global info must be copied here
    setGlobalScale(transform.globalScale());
    setGlobalPosition(transform.globalPosition());

    return *this;
}
}