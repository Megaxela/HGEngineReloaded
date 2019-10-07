
#pragma once

// C++ STl
#include <functional>

// GLM
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace HG::Rendering::Base
{
class Texture;
class CubeMap;

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
        IntVector2,
        IntVector3,
        IntVector4,
        Matrix2x2,
        Matrix3x3,
        Matrix4x4,
        Texture,
        CubeMap
    };

    Type type;

    union
    {
        int integer;
        float floating;
        bool boolean;
        glm::vec2 vector2;
        glm::vec3 vector3;
        glm::vec4 vector4;
        glm::ivec2 vector2_int;
        glm::ivec3 vector3_int;
        glm::ivec4 vector4_int;
        glm::mat2 mat2x2;
        glm::mat3 mat3x3;
        glm::mat4 mat4x4;
        HG::Rendering::Base::Texture* texture;
        HG::Rendering::Base::CubeMap* cubeMap;
    };
};
} // namespace HG::Rendering::Base

namespace std
{
template <>
struct hash<HG::Rendering::Base::MaterialValue>
{
    std::size_t operator()(const HG::Rendering::Base::MaterialValue& value)
    {
        std::size_t result = 17;
        auto typeHash      = std::hash<int>()(static_cast<int>(value.type));

        std::size_t valueHash;

        // For magic constants 17 and 32 see
        // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
        switch (value.type)
        {
        case HG::Rendering::Base::MaterialValue::Type::Int:
            valueHash = std::hash<int>()(value.integer);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Float:
            valueHash = std::hash<float>()(value.floating);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Boolean:
            valueHash = std::hash<bool>()(value.boolean);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Vector2:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.vector2.x);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector2.y);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Vector3:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.vector3.x);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector3.y);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector3.z);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Vector4:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.vector4.x);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector4.y);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector4.z);
            valueHash = valueHash * 31 + std::hash<float>()(value.vector4.w);
            break;
        case HG::Rendering::Base::MaterialValue::Type::IntVector2:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<int>()(value.vector2_int.x);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector2_int.y);
            break;
        case HG::Rendering::Base::MaterialValue::Type::IntVector3:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<int>()(value.vector3_int.x);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector3_int.y);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector3_int.z);
            break;
        case HG::Rendering::Base::MaterialValue::Type::IntVector4:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<int>()(value.vector4_int.x);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector4_int.y);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector4_int.z);
            valueHash = valueHash * 31 + std::hash<int>()(value.vector4_int.w);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Matrix2x2:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.mat2x2[0][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat2x2[0][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat2x2[1][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat2x2[1][1]);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Matrix3x3:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[0][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[0][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[0][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[1][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[1][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[1][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[2][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[2][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat3x3[2][2]);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Matrix4x4:
            valueHash = 17;
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[0][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[0][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[0][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[0][3]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[1][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[1][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[1][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[1][3]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[2][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[2][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[2][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[2][3]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[3][0]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[3][1]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[3][2]);
            valueHash = valueHash * 31 + std::hash<float>()(value.mat4x4[3][3]);
            break;
        case HG::Rendering::Base::MaterialValue::Type::Texture:
            valueHash = std::hash<void*>()(value.texture);
            break;
        case HG::Rendering::Base::MaterialValue::Type::CubeMap:
            valueHash = std::hash<void*>()(value.cubeMap);
            break;
        default:
            throw std::invalid_argument("Unknown type for hashing");
        }

        result = result * 31 + typeHash;
        result = result * 31 + valueHash;

        return result;
    }
};
} // namespace std