// HG::Rendering::Base
#include <CubeMap.hpp>

HG::Rendering::Base::CubeMap::CubeMap() :
    RenderData(Id),
    m_right(nullptr),
    m_left(nullptr),
    m_top(nullptr),
    m_bottom(nullptr),
    m_front(nullptr),
    m_back(nullptr)
{

}

HG::Rendering::Base::CubeMap::CubeMap(HG::Utils::SurfacePtr right,
                                                    HG::Utils::SurfacePtr left,
                                                    HG::Utils::SurfacePtr top,
                                                    HG::Utils::SurfacePtr bottom,
                                                    HG::Utils::SurfacePtr front,
                                                    HG::Utils::SurfacePtr back) :
    RenderData(Id),
    m_right(std::move(right)),
    m_left(std::move(left)),
    m_top(std::move(top)),
    m_bottom(std::move(bottom)),
    m_front(std::move(front)),
    m_back(std::move(back))
{

}

HG::Utils::SurfacePtr HG::Rendering::Base::CubeMap::getSideSurface(HG::Rendering::Base::CubeMap::Side side) const
{
    switch (side)
    {
    case Right:  return m_right  ;
    case Left:   return m_left   ;
    case Top:    return m_top    ;
    case Bottom: return m_bottom ;
    case Front:  return m_front  ;
    case Back:   return m_back   ;
    }

    return nullptr;
}

void HG::Rendering::Base::CubeMap::setSideSurface(HG::Rendering::Base::CubeMap::Side side,
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
