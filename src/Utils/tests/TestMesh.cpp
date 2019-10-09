// HG::Utils
#include <HG/Utils/Mesh.hpp>

// GTest
#include <gtest/gtest.h>

TEST(Utils, MeshTangentBitangent)
{
    HG::Utils::Mesh mesh;

    float scale   = 1.0f;
    mesh.Vertices = {
        {{-0.5f * scale, -0.5f * scale, 0}, {1.0f, 1.0f}},
        {{0.5f * scale, 0.5f * scale, 0}, {0.0f, 0.0f}},
        {{0.5f * scale, -0.5f * scale, 0}, {0.0f, 1.0f}},
        {{0.5f * scale, 0.5f * scale, 0}, {0.0f, 0.0f}},
        {{-0.5f * scale, -0.5f * scale, 0}, {1.0f, 1.0f}},
        {{-0.5f * scale, 0.5f * scale, 0}, {1.0f, 0.0f}},
    };

    mesh.Indices = {0, 1, 2, 3, 4, 5};

    std::vector<std::pair<glm::vec3, glm::vec3>> expected = {
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        std::make_pair(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
    };

    ASSERT_NO_THROW(mesh.calculateTangentBitangentVectors());

    for (size_t i = 0; i < mesh.Vertices.size(); ++i)
    {
        ASSERT_EQ(mesh.Vertices[i].tangent, expected[i].first);
        ASSERT_EQ(mesh.Vertices[i].bitangent, expected[i].second);
    }
}
