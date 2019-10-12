#pragma once

// C++ STL
#include <memory>
#include <vector>

// HG::Utils
#include <HG/Utils/Vertex.hpp>

namespace HG::Utils
{
/**
 * @brief Class, that describes
 * mesh with mesh data.
 */
class Mesh
{
public:
    // Properties
    std::vector<Vertex> Vertices;
    std::vector<std::uint32_t> Indices;

    /**
     * @brief Default constructor.
     */
    Mesh();

    /**
     * @brief Method for calculating tangent and
     * bitangent vectors of mesh. You can
     * define this vectors with your own.
     */
    void calculateTangentBitangentVectors();
};

using MeshPtr = std::shared_ptr<Mesh>;
} // namespace HG::Utils
