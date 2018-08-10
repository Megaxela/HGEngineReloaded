#include "CubeMapTexture.hpp"

HG::Rendering::Base::CubeMapTexture::CubeMapTexture() :
    m_externalData(nullptr),
    m_right(nullptr),
    m_left(nullptr),
    m_top(nullptr),
    m_bottom(nullptr),
    m_front(nullptr),
    m_back(nullptr)
{

}

HG::Rendering::Base::CubeMapTexture::CubeMapTexture(HG::Utils::SurfacePtr right,
                                                         HG::Utils::SurfacePtr left,
                                                         HG::Utils::SurfacePtr top,
                                                         HG::Utils::SurfacePtr bottom,
                                                         HG::Utils::SurfacePtr front,
                                                         HG::Utils::SurfacePtr back) :
    m_externalData(nullptr),
    m_right(std::move(right)),
    m_left(std::move(left)),
    m_top(std::move(top)),
    m_bottom(std::move(bottom)),
    m_front(std::move(front)),
    m_back(std::move(back))
{

}

HG::Rendering::Base::CubeMapTexture::~CubeMapTexture()
{
    clearExternalData();
}

void HG::Rendering::Base::CubeMapTexture::clearExternalData()
{
    delete m_externalData;
    m_externalData = nullptr;
}

HG::Utils::SurfacePtr
HG::Rendering::Base::CubeMapTexture::getSideSurface(HG::Rendering::Base::CubeMapTexture::Side side) const
{
    switch (side)
    {
    case Right:  return m_right;  break;
    case Left:   return m_left;   break;
    case Top:    return m_top;    break;
    case Bottom: return m_bottom; break;
    case Front:  return m_front;  break;
    case Back:   return m_back;   break;
    }

    return nullptr;
}

void HG::Rendering::Base::CubeMapTexture::setSideSurface(HG::Rendering::Base::CubeMapTexture::Side side,
                                                              HG::Utils::SurfacePtr surface)
{
    switch (side)
    {
    case Right:  m_right  = surface; break;
    case Left:   m_left   = surface; break;
    case Top:    m_top    = surface; break;
    case Bottom: m_bottom = surface; break;
    case Front:  m_front  = surface; break;
    case Back:   m_back   = surface; break;
    }

    throw std::invalid_argument("Wrong side value.");
}
