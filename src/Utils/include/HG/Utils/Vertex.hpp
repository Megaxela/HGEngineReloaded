#pragma once

// GLM
#include <glm/glm.hpp>

namespace HG::Utils
{
    /**
     * @brief Struct, that describes vertex.
     */
    struct Vertex
    {
        /**
         * @brief Default constructor.
         */
        Vertex() :
            position(),
            normal(),
            uv(),
            tangent(),
            bitangent()
        {}

        /**
         * @brief Simple constructor from
         * pos and uv.
         * @param pos Position.
         * @param uv UV.
         */
        Vertex(const glm::vec3& pos,
               const glm::vec2& uv) :
            position(pos),
            normal(0.0f),
            uv(uv),
            tangent(0.0f),
            bitangent(0.0f)
        {}

        /**
         * @brief Constructor from position,
         * normal vector and uv.
         * @param pos Position.
         * @param normal Normal vector.
         * @param uv UV.
         */
        Vertex(const glm::vec3& pos,
               const glm::vec3& normal,
               const glm::vec2& uv) :
            position(pos),
            normal(normal),
            uv(uv),
            tangent(0.0f),
            bitangent(0.0f)
        {

        }

        /**
         * @brief Constructor from
         * position, normal vector
         * uv coords, tangent and bitangent values.
         * @param pos Position.
         * @param normal Normal vector.
         * @param uv UV.
         * @param tangent Tangent value.
         * @param bitangent Bitangent value.
         */
        Vertex(const glm::vec3& pos,
               const glm::vec3& normal,
               const glm::vec2& uv,
               const glm::vec3& tangent,
               const glm::vec3& bitangent) :
            position(pos),
            normal(normal),
            uv(uv),
            tangent(tangent),
            bitangent(bitangent)
        {

        }


        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;

        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
}

