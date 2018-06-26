#include "CubeMapTexture.hpp"

RENDERING_BASE_MODULE_NS::CubeMapTexture::CubeMapTexture() :
    m_externalData(nullptr),
    m_right(nullptr),
    m_left(nullptr),
    m_top(nullptr),
    m_bottom(nullptr),
    m_front(nullptr),
    m_back(nullptr)
{

}

RENDERING_BASE_MODULE_NS::CubeMapTexture::CubeMapTexture(::UTILS_MODULE_NS::SurfacePtr right,
                                                         ::UTILS_MODULE_NS::SurfacePtr left,
                                                         ::UTILS_MODULE_NS::SurfacePtr top,
                                                         ::UTILS_MODULE_NS::SurfacePtr bottom,
                                                         ::UTILS_MODULE_NS::SurfacePtr front,
                                                         ::UTILS_MODULE_NS::SurfacePtr back) :
    m_externalData(nullptr),
    m_right(std::move(right)),
    m_left(std::move(left)),
    m_top(std::move(top)),
    m_bottom(std::move(bottom)),
    m_front(std::move(front)),
    m_back(std::move(back))
{

}

RENDERING_BASE_MODULE_NS::CubeMapTexture::~CubeMapTexture()
{
    clearExternalData();
}

void RENDERING_BASE_MODULE_NS::CubeMapTexture::clearExternalData()
{
    delete m_externalData;
    m_externalData = nullptr;
}

::UTILS_MODULE_NS::SurfacePtr
RENDERING_BASE_MODULE_NS::CubeMapTexture::getSideSurface(RENDERING_BASE_MODULE_NS::CubeMapTexture::Side side) const
{
    switch (side)
    {
    case Right:  return m_right;
    case Left:   return m_left;
    case Top:    return m_top;
    case Bottom: return m_bottom;
    case Front:  return m_front;
    case Back:   return m_back;
    }

    return nullptr;
}

void RENDERING_BASE_MODULE_NS::CubeMapTexture::setSideSurface(RENDERING_BASE_MODULE_NS::CubeMapTexture::Side side,
                                                              ::UTILS_MODULE_NS::SurfacePtr surface)
{
    switch (side)
    {
    case Right:  m_right  = surface;
    case Left:   m_left   = surface;
    case Top:    m_top    = surface;
    case Bottom: m_bottom = surface;
    case Front:  m_front  = surface;
    case Back:   m_back   = surface;
    }

    throw std::invalid_argument("Wrong side value.");
}
