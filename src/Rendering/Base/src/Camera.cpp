#include <CurrentLogger.hpp>
#include "Camera.hpp"

HG::Rendering::Base::Camera::OrthogonalSettings::OrthogonalSettings() :
    m_zoom(1.0f),
    m_size(1),
    m_parentCam(nullptr)
{

}

HG::Rendering::Base::Camera::OrthogonalSettings::OrthogonalSettings(const HG::Rendering::Base::Camera::OrthogonalSettings& rhs) :
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

HG::Rendering::Base::Camera::OrthogonalSettings& HG::Rendering::Base::Camera::OrthogonalSettings::operator=(const HG::Rendering::Base::Camera::OrthogonalSettings& rhs)
{
    m_zoom = rhs.m_zoom;
    m_size = rhs.m_size;
    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

HG::Rendering::Base::Camera::PerspectiveSettings::PerspectiveSettings() :
    m_fov(80),
    m_parentCam(nullptr)
{

}

HG::Rendering::Base::Camera::PerspectiveSettings::PerspectiveSettings(const HG::Rendering::Base::Camera::PerspectiveSettings& rhs) :
    PerspectiveSettings()
{
    (*this) = rhs;
}

void HG::Rendering::Base::Camera::PerspectiveSettings::setFieldOfView(float fov)
{
    assert(fov >= 1 && fov <= 179);
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

HG::Rendering::Base::Camera::PerspectiveSettings& HG::Rendering::Base::Camera::PerspectiveSettings::operator=(const HG::Rendering::Base::Camera::PerspectiveSettings& rhs)
{
    m_fov = rhs.m_fov;

    if (m_parentCam != nullptr)
    {
        m_parentCam->m_projectionMatrixChanged = true;
    }

    return (*this);
}

const std::vector<HG::Rendering::Base::Camera*>& HG::Rendering::Base::Camera::allCameras()
{
    return m_cameras;
}

std::size_t HG::Rendering::Base::Camera::allCamerasCount()
{
    return m_cameras.size();
}

HG::Rendering::Base::Camera::Camera() :
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
}

HG::Rendering::Base::Camera::~Camera()
{

}

HG::Rendering::Base::Camera::OrthogonalSettings& HG::Rendering::Base::Camera::orthogonalSettings()
{
    return m_orthogonalSettings;
}

HG::Rendering::Base::Camera::PerspectiveSettings& HG::Rendering::Base::Camera::perspectiveSettings()
{
    return m_perspectiveSettings;
}

glm::mat4 HG::Rendering::Base::Camera::projectionMatrix() const
{
    if (!m_projectionMatrixChanged)
    {
        return m_projectionMatrix;
    }

    switch (m_projection)
    {
    case Projection::Perspective:
        m_projectionMatrix = glm::perspective(
            m_perspectiveSettings.fieldOfView(),
            (float) m_viewport.w / (float) m_viewport.h,
            m_near,
            m_far
        );
        m_projectionMatrixChanged = false;
        break;
    case Projection::Orthogonal:
        m_projectionMatrix = glm::ortho(
            (float) m_viewport.x - (m_viewport.w / 2) * m_orthogonalSettings.zoom(),
            (float) m_viewport.w / 2                  * m_orthogonalSettings.zoom(),
            (float) m_viewport.y - (m_viewport.h / 2) * m_orthogonalSettings.zoom(),
            (float) m_viewport.h / 2                  * m_orthogonalSettings.zoom(),
            m_near,
            m_far);
//        m_projectionMatrix = glm::ortho(
//                (float) 0.0f,
//                (float) 1.0f,
//                (float) 0.0f,
//                (float) 1.0f,
//                m_near,
//                m_far);
        m_projectionMatrixChanged = false;
        break;
    default:
        Error() << "Found wrong camera projection value.";
        exit(-3);
    }

    return m_projectionMatrix;
}

void HG::Rendering::Base::Camera::setViewport(int x, int y, int w, int h)
{

}

void HG::Rendering::Base::Camera::setViewport(const HG::Utils::Rect& rect)
{

}

HG::Utils::Rect HG::Rendering::Base::Camera::viewport() const
{
    return HG::Utils::Rect();
}

void HG::Rendering::Base::Camera::setNear(HG::Rendering::Base::Camera::CullType value)
{

}

HG::Rendering::Base::Camera::CullType HG::Rendering::Base::Camera::getNear() const
{
    return 0;
}

void HG::Rendering::Base::Camera::setFar(HG::Rendering::Base::Camera::CullType value)
{

}

HG::Rendering::Base::Camera::CullType HG::Rendering::Base::Camera::getFar() const
{
    return 0;
}
