#pragma once

// C++ STL
#include <memory>

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderData.hpp> // Required for inheritance

// HG::Utils
#include <HG/Utils/FutureHandler.hpp>
#include <HG/Utils/StringTools.hpp>

// ALogger
#include <CurrentLogger.hpp>

namespace HG::Utils
{
class Surface;
using SurfacePtr       = std::shared_ptr<Surface>;
using SurfaceFuturePtr = FutureHandler<SurfacePtr>;
} // namespace HG::Utils

namespace HG::Rendering::Base
{
/**
 * @brief Class, that describes cube map
 * texture for rendering (in GPU / prepared for GPU)
 */
class CubeMap
    : public HG::Core::CachableResource<CubeMap>
    , public HG::Rendering::Base::RenderData
{
public:
    constexpr static std::size_t Id = HG::Utils::StringTools::hash("RenderData::CubeMap");

    /**
     * @brief Cube side.
     */
    enum Side
    {
        Right  = 0,
        Left   = 1,
        Top    = 2,
        Bottom = 3,
        Back   = 4,
        Front  = 5,
        Last
    };

    /**
     * @brief Constructor.
     */
    CubeMap();

    /**
     * @brief Initialize constructor.
     * @param right Right side surface.
     * @param left Left side surface.
     * @param top Top side surface.
     * @param bottom Bottom side surface.
     * @param front Front side surface.
     * @param back Back side surface.
     */
    CubeMap(HG::Utils::SurfaceFuturePtr right,
            HG::Utils::SurfaceFuturePtr left,
            HG::Utils::SurfaceFuturePtr top,
            HG::Utils::SurfaceFuturePtr bottom,
            HG::Utils::SurfaceFuturePtr front,
            HG::Utils::SurfaceFuturePtr back);

    /**
     * @brief Destructor. Clears external data.
     */

    /**
     * @brief Method for getting side surface.
     * @param side Side.
     * @param guarantee If true - if available -
     * method will wait for async side loading
     * before return.
     * @return Smart pointer to surface.
     */
    HG::Utils::SurfacePtr getSideSurface(Side side, bool guarantee = false);

    /**
     * @brief Method for setting side surface.
     * Can throw `std::invalid_argument` if side is
     * wrong.
     * @param side Side.
     * @param surface Smart pointer to surface.
     */
    void setSideSurface(Side side, HG::Utils::SurfaceFuturePtr surface);

private:
    // Surfaces
    HG::Utils::SurfaceFuturePtr m_right;
    HG::Utils::SurfaceFuturePtr m_left;
    HG::Utils::SurfaceFuturePtr m_top;
    HG::Utils::SurfaceFuturePtr m_bottom;
    HG::Utils::SurfaceFuturePtr m_front;
    HG::Utils::SurfaceFuturePtr m_back;
};
} // namespace HG::Rendering::Base
