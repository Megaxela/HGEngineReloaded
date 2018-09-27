#pragma once

// C++ STL
#include <memory> // For IWYU

// HG::Rendering::Base
#include <RenderData.hpp> // Required for inheritance

// ALogger
#include <CurrentLogger.hpp>

namespace HG::Utils
{
    class Surface;
    using SurfacePtr = std::shared_ptr<Surface>;
}

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes cube map
     * texture for rendering (in GPU / prepared for GPU)
     */
    class CubeMap : public RenderData
    {
    public:

        constexpr static std::size_t Id = 4;

        /**
         * @brief Cube side.
         */
        enum Side
        {
            Right = 0,
            Left  = 1,
            Top   = 2,
            Bottom = 3,
            Front  = 4,
            Back   = 5
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
        CubeMap(HG::Utils::SurfacePtr right,
                HG::Utils::SurfacePtr left,
                HG::Utils::SurfacePtr top,
                HG::Utils::SurfacePtr bottom,
                HG::Utils::SurfacePtr front,
                HG::Utils::SurfacePtr back);

        /**
         * @brief Destructor. Clears external data.
         */

        /**
         * @brief Method for getting side surface.
         * @param side Side.
         * @return Smart pointer to surface.
         */
        HG::Utils::SurfacePtr getSideSurface(Side side) const;

        /**
         * @brief Method for setting side surface.
         * Can throw `std::invalid_argument` if side is
         * wrong.
         * @param side Side.
         * @param surface Smart pointer to surface.
         */
        void setSideSurface(Side side, HG::Utils::SurfacePtr surface);

    private:

        // Surfaces
        HG::Utils::SurfacePtr m_right;
        HG::Utils::SurfacePtr m_left;
        HG::Utils::SurfacePtr m_top;
        HG::Utils::SurfacePtr m_bottom;
        HG::Utils::SurfacePtr m_front;
        HG::Utils::SurfacePtr m_back;
    };
}

