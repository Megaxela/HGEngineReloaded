#pragma once

#include <CurrentLogger.hpp>
#include <Surface.hpp>
#include <RenderData.hpp>

namespace HG::Rendering::Base
{
    /**
     * @brief Class, that describes cube map
     * texture for rendering (in GPU / prepared for GPU)
     */
    class CubeMapTexture : public RenderData
    {
    public:

        constexpr static std::size_t Id = 4;

        /**
         * @brief Cube side.
         */
        enum Side
        {
            Right,
            Left,
            Top,
            Bottom,
            Front,
            Back
        };

        /**
         * @brief Constructor.
         */
        CubeMapTexture();

        /**
         * @brief Initialize constructor.
         * @param right Right side surface.
         * @param left Left side surface.
         * @param top Top side surface.
         * @param bottom Bottom side surface.
         * @param front Front side surface.
         * @param back Back side surface.
         */
        CubeMapTexture(HG::Utils::SurfacePtr right,
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

