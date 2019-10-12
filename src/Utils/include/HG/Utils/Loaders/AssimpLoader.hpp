#pragma once

// C++ STL
#include <cstddef>
#include <memory>

namespace HG::Utils
{
class Model;
using ModelPtr = std::shared_ptr<Model>;

/**
 * @brief Models loader for resource manager.
 * Uses assimp library for loading models.
 * See https://github.com/assimp/assimp
 */
class AssimpLoader
{
public:
    using ResultType = ModelPtr;

    /**
     * @brief Constructor.
     */
    AssimpLoader();

    /**
     * @brief Method for loading model from raw
     * file data.
     * Root model is made with shared ptr, but
     * it's children made with raw pointers.
     * @todo Inspect model internal system. Apply smart pointers to parent/children system.
     * @param data Pointer to data.
     * @param size Amount of data.
     * @return Loaded model or nullptr if error acquired.
     */
    ResultType load(const std::byte* data, std::size_t size);
};
} // namespace HG::Utils
