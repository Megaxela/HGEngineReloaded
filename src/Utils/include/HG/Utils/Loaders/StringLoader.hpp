#pragma once

// C++ STL
#include <cstddef>
#include <string>

namespace HG::Utils
{
/**
 * @brief Loader class for loading asset as string.
 */
class StringLoader
{
    using ResultType = std::string;

    /**
     * @brief Constructor.
     */
    StringLoader();

    /**
     * @brief Method for loading image from image format to raw.
     * @param data Pointer to data object.
     * @return Loaded surface or nullptr if error acquired.
     */
    ResultType load(const std::byte* data, std::size_t size);
};
} // namespace HG::Utils
