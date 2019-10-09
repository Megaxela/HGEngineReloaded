#pragma once

// GLM
#include <glm/glm.hpp>

namespace glmex
{
namespace vec3
{
static glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 up      = glm::vec3(0.0f, 1.0f, 0.0f);
static glm::vec3 right   = glm::vec3(1.0f, 0.0f, 0.0f);
} // namespace vec3

template <typename T, glm::qualifier Q>
GLM_FUNC_QUALIFIER glm::vec<4, T, Q> transform_point(glm::vec<4, T, Q> const& point, glm::mat<4, 4, T, Q> const& matrix)
{
    return matrix * point + matrix[3];
}

template <typename T, glm::qualifier Q>
GLM_FUNC_QUALIFIER glm::vec<4, T, Q> transform_vector(glm::vec<4, T, Q> const& point,
                                                      glm::mat<4, 4, T, Q> const& matrix)
{
    return matrix * point;
}
} // namespace glmex

namespace glm
{
template <typename T, qualifier Q>
GLM_FUNC_QUALIFIER mat<4, 4, T, Q> orthonormalize(mat<4, 4, T, Q> const& m)
{
    mat<4, 4, T, Q> r = m;

    r[0] = normalize(r[0]);
    r[1] = normalize(r[1]);
    r[2] = normalize(r[2]);

    return r;
}

template <typename T, qualifier Q>
GLM_FUNC_QUALIFIER vec<3, T, Q> get_scale(mat<4, 4, T, Q> const& m)
{
    return glm::vec3(glm::length(glm::vec3(m[0])), glm::length(glm::vec3(m[1])), glm::length(glm::vec3(m[2])));
}

template <typename T, qualifier Q>
GLM_FUNC_QUALIFIER vec<3, T, Q> get_translate(mat<4, 4, T, Q> const& m)
{
    return glm::vec3(m[3]);
}
} // namespace glm
