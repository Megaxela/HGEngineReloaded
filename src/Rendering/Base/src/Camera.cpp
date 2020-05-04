// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/BuildProperties.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Transform.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/glmex.hpp>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace HG::Rendering::Base
{
Camera::OrthogonalSettings::OrthogonalSettings() : m_zoom(1.0f), m_size(1), m_parentCam(nullptr)
{
}

Camera::OrthogonalSettings::OrthogonalSettings(const Camera::OrthogonalSettings& rhs) : OrthogonalSettings()
{
    (*this) = rhs;
}

void Camera::OrthogonalSettings::setZoom(float zoom)
{
    m_zoom = zoom;
}

float Camera::OrthogonalSettings::zoom() const
{
    return m_zoom;
}

void Camera::OrthogonalSettings::setSize(std::uint32_t size)
{
    m_size = size;
}

std::uint32_t Camera::OrthogonalSettings::size() const
{
    return m_size;
}

Camera::OrthogonalSettings& Camera::OrthogonalSettings::operator=(const Camera::OrthogonalSettings& rhs)
{
    m_zoom = rhs.m_zoom;
    m_size = rhs.m_size;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

Camera::PerspectiveSettings::PerspectiveSettings() : m_fov(90), m_parentCam(nullptr)
{
}

Camera::PerspectiveSettings::PerspectiveSettings(const Camera::PerspectiveSettings& rhs) : PerspectiveSettings()
{
    (*this) = rhs;
}

void Camera::PerspectiveSettings::setFieldOfView(float fov)
{
    m_fov = fov;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }
}

float Camera::PerspectiveSettings::fieldOfView() const
{
    return m_fov;
}

Camera::PerspectiveSettings& Camera::PerspectiveSettings::operator=(const Camera::PerspectiveSettings& rhs)
{
    m_fov = rhs.m_fov;

    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

Camera::Camera() :
    m_projectionMatrixChanged(true),
    m_projectionMatrix(1.0f),
    m_projection(),
    m_near(0.1f),
    m_far(1000.0f),
    m_orthogonalSettings(),
    m_perspectiveSettings()
{
    m_orthogonalSettings.m_parentCam  = this;
    m_perspectiveSettings.m_parentCam = this;
}

Camera::~Camera()
{
    // If active camera removed, clean value
    if (scene() && scene()->application() && scene()->application()->renderer() &&
        scene()->application()->renderer()->activeCamera() == this)
    {
        scene()->application()->renderer()->setActiveCamera(nullptr);
    }
}

Camera::OrthogonalSettings& Camera::orthogonalSettings()
{
    return m_orthogonalSettings;
}

Camera::PerspectiveSettings& Camera::perspectiveSettings()
{
    return m_perspectiveSettings;
}

glm::mat4 Camera::viewMatrix() const
{
    auto parentGO = gameObject();
    if (!parentGO)
    {
        HGError("Can't get game object of active camera.");
        return glm::mat4();
    }

    glm::vec3 pos = parentGO->transform()->globalPosition();
    glm::quat rot = parentGO->transform()->globalRotation();

    glm::vec3 front = glmex::vec3::forward * rot;

    return glm::lookAt(pos, pos + front, glmex::vec3::up);
}

void Camera::setProjection(Camera::Projection projection)
{
    if (m_projection != projection)
    {
        m_projectionMatrixChanged = true;
    }

    m_projection = projection;
}

Camera::Projection Camera::projection() const
{
    return m_projection;
}

glm::mat4 Camera::projectionMatrix() const
{
    auto viewportSize = scene()->application()->renderer()->pipeline()->renderTarget()->size(); // Current render target

    if (!m_projectionMatrixChanged && m_cachedViewportSize == viewportSize)
    {
        return m_projectionMatrix;
    }

    m_cachedViewportSize = viewportSize;

    switch (m_projection)
    {
    case Projection::Perspective:
        m_projectionMatrix        = glm::perspective(glm::radians(m_perspectiveSettings.fieldOfView()),
                                              (float)viewportSize.x / (float)viewportSize.y,
                                              m_near,
                                              m_far);
        m_projectionMatrixChanged = false;
        break;
    case Projection::Orthogonal: {
        auto metersPerHighestSide = 10;

        auto width  = viewportSize.x / (float)std::min(viewportSize.x, viewportSize.y) * metersPerHighestSide;
        auto height = viewportSize.y / (float)std::min(viewportSize.x, viewportSize.y) * metersPerHighestSide;

        m_projectionMatrix = glm::ortho((float)-(width / 2.0f) * m_orthogonalSettings.zoom(),
                                        (float)width / 2 * m_orthogonalSettings.zoom(),
                                        (float)-(height / 2.0f) * m_orthogonalSettings.zoom(),
                                        (float)height / 2 * m_orthogonalSettings.zoom(),
                                        m_near,
                                        m_far);
    }
        m_projectionMatrixChanged = false;
        break;
    default:
        HGError("Found wrong camera projection value");
        throw std::runtime_error("Found wrong camera projection value");
    }

    return m_projectionMatrix;
}

void Camera::setNear(Camera::CullType value)
{
    m_near = value;
}

Camera::CullType Camera::getNear() const
{
    return m_near;
}

void Camera::setFar(Camera::CullType value)
{
    m_far = value;
}

Camera::CullType Camera::getFar() const
{
    return m_far;
}

void Camera::onStart()
{
    if constexpr (HG::Core::BuildProperties::isDebug())
    {
        if (gameObject() == nullptr)
        {
            throw std::runtime_error("Starting camera, w/o GameObject.");
        }

        // Getting current viewport
        if (scene() == nullptr)
        {
            HGError("No scene set. Can't determine initial viewport.");
            return;
        }

        if (scene()->application() == nullptr)
        {
            HGError("No application set in scene. Can't determine initial viewport.");
            return;
        }

        if (scene()->application()->renderer() == nullptr)
        {
            HGError("No renderer set for application.");
            return;
        }

        if (scene()->application()->systemController() == nullptr)
        {
            HGError("No system controller set in application. Can't determine initial viewport.");
            return;
        }
    }

    // If there is no active camera, set this.
    if (!scene()->application()->renderer()->activeCamera())
    {
        setActive();
    }
}

void Camera::lookAt(const glm::vec3& point)
{
    lookAt(point, glmex::vec3::up);
}

void Camera::lookAt(const glm::vec3& point, const glm::vec3& upVector)
{
    auto position     = gameObject()->transform()->localPosition();
    auto resultMatrix = glm::lookAt(point, position, upVector);

    // Taking data back
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(resultMatrix, scale, rotation, translation, skew, perspective);

    if constexpr (HG::Core::BuildProperties::isDebug())
    {
        if (gameObject() == nullptr)
        {
            HGError("Can't make camera look at without gameobject.");
            return;
        }
    }

    gameObject()->transform()->setLocalRotation(rotation);
}

void Camera::setActive()
{
    scene()->application()->renderer()->setActiveCamera(this);
}
} // namespace HG::Rendering::Base
