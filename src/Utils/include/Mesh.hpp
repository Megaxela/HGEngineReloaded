#pragma once

#include <vector>
#include "Vertex.hpp"

namespace UTILS_MODULE_NS
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
        std::vector<uint32_t> Indices;

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
}



