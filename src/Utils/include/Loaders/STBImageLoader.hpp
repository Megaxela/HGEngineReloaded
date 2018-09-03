#pragma once

#include <cstddef>
#include <memory>

namespace HG::Utils
{
    class Surface;
    using SurfacePtr = std::shared_ptr<Surface>;

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
         * @brief Method for loading image from raw data.
         * @param data Pointer to data.
         * @param size Amount of data.
         * @return Loaded surface or nullptr if error acquired.
         */
        ResultType load(const std::byte* data, std::size_t size);

    };
}
