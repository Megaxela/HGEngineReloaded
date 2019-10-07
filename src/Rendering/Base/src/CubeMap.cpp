// HG::Rendering::Base
#include <HG/Rendering/Base/CubeMap.hpp>

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

HG::Rendering::Base::CubeMap::CubeMap(HG::Utils::SurfaceFuturePtr right,
                                      HG::Utils::SurfaceFuturePtr left,
                                      HG::Utils::SurfaceFuturePtr top,
                                      HG::Utils::SurfaceFuturePtr bottom,
                                      HG::Utils::SurfaceFuturePtr front,
                                      HG::Utils::SurfaceFuturePtr back) :
    RenderData(Id),
    m_right(std::move(right)),
    m_left(std::move(left)),
    m_top(std::move(top)),
    m_bottom(std::move(bottom)),
    m_front(std::move(front)),
    m_back(std::move(back))
{
}

HG::Utils::SurfacePtr HG::Rendering::Base::CubeMap::getSideSurface(HG::Rendering::Base::CubeMap::Side side,
                                                                   bool guarantee)
{
    if (guarantee)
    {
        switch (side)
        {
        case Right:
            return m_right.guaranteeGet();
        case Left:
            return m_left.guaranteeGet();
        case Top:
            return m_top.guaranteeGet();
        case Bottom:
            return m_bottom.guaranteeGet();
        case Front:
            return m_front.guaranteeGet();
        case Back:
            return m_back.guaranteeGet();
        default:
            break;
        }
    }
    else
    {
        switch (side)
        {
        case Right:
            return m_right.get();
        case Left:
            return m_left.get();
        case Top:
            return m_top.get();
        case Bottom:
            return m_bottom.get();
        case Front:
            return m_front.get();
        case Back:
            return m_back.get();
        default:
            break;
        }
    }

    return nullptr;
}

void HG::Rendering::Base::CubeMap::setSideSurface(HG::Rendering::Base::CubeMap::Side side,
                                                  HG::Utils::SurfaceFuturePtr surface)
{
    switch (side)
    {
    case Right:
        m_right = std::move(surface);
        break;
    case Left:
        m_left = std::move(surface);
        break;
    case Top:
        m_top = std::move(surface);
        break;
    case Bottom:
        m_bottom = std::move(surface);
        break;
    case Front:
        m_front = std::move(surface);
        break;
    case Back:
        m_back = std::move(surface);
        break;
    default:
        break;
    }

    throw std::invalid_argument("Wrong side value.");
}
