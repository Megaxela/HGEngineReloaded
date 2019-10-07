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
#include <HG/Utils/glmex.hpp>

// ALogger
#include <CurrentLogger.hpp>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

HG::Rendering::Base::Camera::OrthogonalSettings::OrthogonalSettings() : m_zoom(1.0f), m_size(1), m_parentCam(nullptr)
{
}

HG::Rendering::Base::Camera::OrthogonalSettings::OrthogonalSettings(
    const HG::Rendering::Base::Camera::OrthogonalSettings& rhs) :
    OrthogonalSettings()
{
    (*this) = rhs;
}

void HG::Rendering::Base::Camera::OrthogonalSettings::setZoom(float zoom)
{
    m_zoom = zoom;
}

float HG::Rendering::Base::Camera::OrthogonalSettings::zoom() const
{
    return m_zoom;
}

void HG::Rendering::Base::Camera::OrthogonalSettings::setSize(uint32_t size)
{
    m_size = size;
}

uint32_t HG::Rendering::Base::Camera::OrthogonalSettings::size() const
{
    return m_size;
}

HG::Rendering::Base::Camera::OrthogonalSettings&
HG::Rendering::Base::Camera::OrthogonalSettings::operator=(const HG::Rendering::Base::Camera::OrthogonalSettings& rhs)
{
    m_zoom = rhs.m_zoom;
    m_size = rhs.m_size;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

HG::Rendering::Base::Camera::PerspectiveSettings::PerspectiveSettings() : m_fov(90), m_parentCam(nullptr)
{
}

HG::Rendering::Base::Camera::PerspectiveSettings::PerspectiveSettings(
    const HG::Rendering::Base::Camera::PerspectiveSettings& rhs) :
    PerspectiveSettings()
{
    (*this) = rhs;
}

void HG::Rendering::Base::Camera::PerspectiveSettings::setFieldOfView(float fov)
{
    m_fov = fov;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }
}

float HG::Rendering::Base::Camera::PerspectiveSettings::fieldOfView() const
{
    return m_fov;
}

HG::Rendering::Base::Camera::PerspectiveSettings&
HG::Rendering::Base::Camera::PerspectiveSettings::operator=(const HG::Rendering::Base::Camera::PerspectiveSettings& rhs)
{
    m_fov = rhs.m_fov;

    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

HG::Rendering::Base::Camera::Camera() :
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

HG::Rendering::Base::Camera::~Camera()
{
    // If active camera removed, clean value
    if (scene() && scene()->application() && scene()->application()->renderer() &&
        scene()->application()->renderer()->activeCamera() == this)
    {
        scene()->application()->renderer()->setActiveCamera(nullptr);
    }
}

HG::Rendering::Base::Camera::OrthogonalSettings& HG::Rendering::Base::Camera::orthogonalSettings()
{
    return m_orthogonalSettings;
}

HG::Rendering::Base::Camera::PerspectiveSettings& HG::Rendering::Base::Camera::perspectiveSettings()
{
    return m_perspectiveSettings;
}

glm::mat4 HG::Rendering::Base::Camera::viewMatrix() const
{
    auto parentGO = gameObject();
    if (!parentGO)
    {
        Error() << "Can't get game object of active camera.";
        return glm::mat4();
    }

    glm::vec3 pos = parentGO->transform()->globalPosition();
    glm::quat rot = parentGO->transform()->globalRotation();

    glm::vec3 front = glmex::vec3::forward * rot;

    return glm::lookAt(pos, pos + front, glmex::vec3::up);
}

void HG::Rendering::Base::Camera::setProjection(HG::Rendering::Base::Camera::Projection projection)
{
    if (m_projection != projection)
    {
        m_projectionMatrixChanged = true;
    }

    m_projection = projection;
}

HG::Rendering::Base::Camera::Projection HG::Rendering::Base::Camera::projection() const
{
    return m_projection;
}

glm::mat4 HG::Rendering::Base::Camera::projectionMatrix() const
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
        Error() << "Found wrong camera projection value.";
        throw std::runtime_error("Found wrong camera projection value.");
    }

    return m_projectionMatrix;
}

void HG::Rendering::Base::Camera::setNear(HG::Rendering::Base::Camera::CullType value)
{
    m_near = value;
}

HG::Rendering::Base::Camera::CullType HG::Rendering::Base::Camera::getNear() const
{
    return m_near;
}

void HG::Rendering::Base::Camera::setFar(HG::Rendering::Base::Camera::CullType value)
{
    m_far = value;
}

HG::Rendering::Base::Camera::CullType HG::Rendering::Base::Camera::getFar() const
{
    return m_far;
}

void HG::Rendering::Base::Camera::onStart()
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
            Error() << "No scene set. Can't determine initial viewport.";
            return;
        }

        if (scene()->application() == nullptr)
        {
            Error() << "No application set in scene. Can't determine initial viewport.";
            return;
        }

        if (scene()->application()->renderer() == nullptr)
        {
            Error() << "No renderer set for application.";
            return;
        }

        if (scene()->application()->systemController() == nullptr)
        {
            Error() << "No system controller set in application. Can't determine initial viewport.";
            return;
        }
    }

    // If there is no active camera, set this.
    if (!scene()->application()->renderer()->activeCamera())
    {
        setActive();
    }
}

void HG::Rendering::Base::Camera::lookAt(const glm::vec3& point)
{
    lookAt(point, glmex::vec3::up);
}

void HG::Rendering::Base::Camera::lookAt(const glm::vec3& point, const glm::vec3& upVector)
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
            Error() << "Can't make camera look at without gameobject.";
            return;
        }
    }

    gameObject()->transform()->setLocalRotation(rotation);
}

void HG::Rendering::Base::Camera::setActive()
{
    scene()->application()->renderer()->setActiveCamera(this);
}
