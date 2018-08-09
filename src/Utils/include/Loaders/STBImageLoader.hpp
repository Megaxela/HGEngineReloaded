#pragma once

#include <Surface.hpp>
#include <cstddef>

namespace HG::Utils
{
    /**
     * @brief Image loader for resource manager.
     */
    class STBImageLoader
    {
    public:

        using ResultType = SurfacePtr;

        /**
         * @brief Constructor.
         */
        STBImageLoader();

        /**
         * @brief Method for loading image from image format to raw.
         * @param data Pointer to data object.
         * @return Loaded surface or nullptr if error acquired.
         */
        ResultType load(const std::byte* data, std::size_t size);

    };
}
