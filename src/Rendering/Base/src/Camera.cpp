#include <CurrentLogger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include <GameObject.hpp>
#include <glmex.hpp>
#include <glm/gtx/matrix_decompose.hpp>

std::vector<RENDERING_BASE_MODULE_NS::Camera*> RENDERING_BASE_MODULE_NS::Camera::m_cameras;
RENDERING_BASE_MODULE_NS::Camera* RENDERING_BASE_MODULE_NS::Camera::m_active = nullptr;

RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::OrthogonalSettings() :
    m_zoom(1.0f),
    m_size(1),
    m_parentCam(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::OrthogonalSettings(
    const RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings& rhs
) :
    OrthogonalSettings()
{
    (*this) = rhs;
}

void RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::setZoom(float zoom)
{
    m_zoom = zoom;
}

float RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::zoom() const
{
    return m_zoom;
}

void RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::setSize(uint32_t size)
{
    m_size = size;
}

uint32_t RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::size() const
{
    return m_size;
}

RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings& RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings::operator=(
    const RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings& rhs
)
{
    m_zoom = rhs.m_zoom;
    m_size = rhs.m_size;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings::PerspectiveSettings() :
    m_fov(90),
    m_parentCam(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings::PerspectiveSettings(
    const RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings& rhs
) :
    PerspectiveSettings()
{
    (*this) = rhs;
}

void RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings::setFieldOfView(float fov)
{
    assert(fov >= 1 && fov <= 179);
    m_fov = fov;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }
}

float RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings::fieldOfView() const
{
    return m_fov;
}

RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings& RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings::operator=(
    const RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings& rhs
)
{
    m_fov = rhs.m_fov;

    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

RENDERING_BASE_MODULE_NS::Camera* RENDERING_BASE_MODULE_NS::Camera::active()
{
    return m_active;
}

const std::vector<RENDERING_BASE_MODULE_NS::Camera*>& RENDERING_BASE_MODULE_NS::Camera::allCameras()
{
    return m_cameras;
}

std::size_t RENDERING_BASE_MODULE_NS::Camera::allCamerasCount()
{
    return m_cameras.size();
}

RENDERING_BASE_MODULE_NS::Camera::Camera() :
    m_projectionMatrixChanged(true),
    m_projectionMatrix(1.0f),
    m_projection(),
    m_viewport({0, 0, 1, 1}),
    m_near(0.1f),
    m_far(1000.0f),
    m_orthogonalSettings(),
    m_perspectiveSettings()
{
    m_orthogonalSettings.m_parentCam = this;
    m_perspectiveSettings.m_parentCam = this;

    // First added camera has to be first
    if (m_active == nullptr)
    {
        m_active = this;
    }
}

RENDERING_BASE_MODULE_NS::Camera::~Camera()
{
    Camera::m_cameras.erase(
        std::find(
            Camera::m_cameras.begin(),
            Camera::m_cameras.end(),
            this
        ),
        Camera::m_cameras.end()
    );

    // If active camera removed, clean value
    if (m_active == this)
    {
        m_active = nullptr;
    }
}

RENDERING_BASE_MODULE_NS::Camera::OrthogonalSettings& RENDERING_BASE_MODULE_NS::Camera::orthogonalSettings()
{
    return m_orthogonalSettings;
}

RENDERING_BASE_MODULE_NS::Camera::PerspectiveSettings& RENDERING_BASE_MODULE_NS::Camera::perspectiveSettings()
{
    return m_perspectiveSettings;
}

glm::mat4 RENDERING_BASE_MODULE_NS::Camera::viewMatrix() const
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

void RENDERING_BASE_MODULE_NS::Camera::setProjection(RENDERING_BASE_MODULE_NS::Camera::Projection projection)
{
    if (m_projection != projection)
    {
        m_projectionMatrixChanged = true;
    }

    m_projection = projection;
}

RENDERING_BASE_MODULE_NS::Camera::Projection HG::Rendering::Base::Camera::projection() const
{
    return m_projection;
}

glm::mat4 RENDERING_BASE_MODULE_NS::Camera::projectionMatrix() const
{
    if (!m_projectionMatrixChanged)
    {
        return m_projectionMatrix;
    }

    switch (m_projection)
    {
    case Projection::Perspective:
        m_projectionMatrix = glm::perspective(
            glm::radians(m_perspectiveSettings.fieldOfView()),
            (float) m_viewport.w / (float) m_viewport.h,
            m_near,
            m_far
        );
        m_projectionMatrixChanged = false;
        break;
    case Projection::Orthogonal:
    {
        auto metersPerHighestSide = 10;

        auto width =  m_viewport.w / (float) std::min(m_viewport.w, m_viewport.h) * metersPerHighestSide;
        auto height = m_viewport.h / (float) std::min(m_viewport.w, m_viewport.h) * metersPerHighestSide;

        m_projectionMatrix = glm::ortho(
            (float) -(width / 2.0f)  * m_orthogonalSettings.zoom(),
            (float)   width / 2      * m_orthogonalSettings.zoom(),
            (float) -(height / 2.0f) * m_orthogonalSettings.zoom(),
            (float)   height / 2     * m_orthogonalSettings.zoom(),
            m_near,
            m_far
        );
    }
        m_projectionMatrixChanged = false;
        break;
    default:
        Error() << "Found wrong camera projection value.";
        throw std::runtime_error("Found wrong camera projection value.");
    }

    return m_projectionMatrix;
}

void RENDERING_BASE_MODULE_NS::Camera::setViewport(int x, int y, int w, int h)
{
    m_viewport.x = x;
    m_viewport.y = y;
    m_viewport.w = w;
    m_viewport.h = h;
    m_projectionMatrixChanged = true;
}

void RENDERING_BASE_MODULE_NS::Camera::setViewport(const HG::Utils::Rect& rect)
{
    m_viewport = rect;
}

HG::Utils::Rect RENDERING_BASE_MODULE_NS::Camera::viewport() const
{
    return m_viewport;
}

void RENDERING_BASE_MODULE_NS::Camera::setNear(RENDERING_BASE_MODULE_NS::Camera::CullType value)
{
    m_near = value;
}

RENDERING_BASE_MODULE_NS::Camera::CullType RENDERING_BASE_MODULE_NS::Camera::getNear() const
{
    return m_near;
}

void RENDERING_BASE_MODULE_NS::Camera::setFar(RENDERING_BASE_MODULE_NS::Camera::CullType value)
{
    m_far = value;
}

RENDERING_BASE_MODULE_NS::Camera::CullType RENDERING_BASE_MODULE_NS::Camera::getFar() const
{
    return m_far;
}

void RENDERING_BASE_MODULE_NS::Camera::onStart()
{
#ifndef NDEBUG

    if (std::find(
        Camera::m_cameras.begin(),
        Camera::m_cameras.end(),
        this
    ) != Camera::m_cameras.end())
    {
        Error() << "Trying to add camera to global camera system, several times.";
    }

#endif
    Camera::m_cameras.push_back(this);

    // Getting current viewport
#ifndef NDEBUG
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

    if (scene()->application()->systemController() == nullptr)
    {
        Error() << "No system controller set in application. Can't determine initial viewport.";
        return;
    }
#endif

    auto viewport = scene()->application()->systemController()->viewport();
    m_viewport = viewport;
}

void RENDERING_BASE_MODULE_NS::Camera::lookAt(const glm::vec3& point)
{
    lookAt(point, glmex::vec3::up);
}

void RENDERING_BASE_MODULE_NS::Camera::lookAt(const glm::vec3& point, const glm::vec3& upVector)
{
    auto position = gameObject()->transform()->localPosition();
    auto resultMatrix = glm::lookAt(point, position, upVector);

    // Taking data back
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(resultMatrix, scale, rotation, translation, skew, perspective);

#ifndef NDEBUG

    if (gameObject() == nullptr)
    {
        Error() << "Can't make camera look at without gameobject.";
        return;
    }

#endif

    gameObject()->transform()->setLocalRotation(rotation);
}
