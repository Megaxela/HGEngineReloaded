
#pragma once

// GLM
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_mat4x4.hpp>

namespace HG::Rendering::Base
{
    class Texture;

    /**
     * @brief Struct, that describes
     * material variable value.
     */
    struct MaterialValue
    {
        enum class Type
        {
            Int,
            Float,
            Boolean,
            Vector2,
            Vector3,
            Vector4,
            Matrix2x2,
            Matrix3x3,
            Matrix4x4,
            Texture,
        };

        Type type;

        union {
            int integer;
            float floating;
            bool boolean;
            glm::vec2 vector2;
            glm::vec3 vector3;
            glm::vec4 vector4;
            glm::mat2 mat2x2;
            glm::mat3 mat3x3;
            glm::mat4 mat4x4;
            HG::Rendering::Base::Texture* texture;
        };
    };
}