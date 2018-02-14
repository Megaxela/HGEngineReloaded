#include "Mesh.hpp"

HG::Utils::Mesh::Mesh() :
    Vertices(),
    Indices()
{

}

void HG::Utils::Mesh::calculateTangentBitangentVectors()
{
    glm::vec3 tangent;
    glm::vec3 bitangent;

    glm::vec3 pos1;
    glm::vec3 pos2;
    glm::vec3 pos3;

    glm::vec2 uv1;
    glm::vec2 uv2;
    glm::vec2 uv3;

    glm::vec3 edge1;
    glm::vec3 edge2;
    glm::vec2 deltaUV1;
    glm::vec2 deltaUV2;

    // Iterating over indicies
    for (size_t indexIndex = 0;
         indexIndex < Indices.size();
         indexIndex += 3)
    {
        assert(indexIndex + 3 <= Indices.size());

        pos1 = Vertices[Indices[indexIndex    ]].position;
        pos2 = Vertices[Indices[indexIndex + 1]].position;
        pos3 = Vertices[Indices[indexIndex + 2]].position;

        uv1 = Vertices[Indices[indexIndex    ]].uv;
        uv2 = Vertices[Indices[indexIndex + 1]].uv;
        uv3 = Vertices[Indices[indexIndex + 2]].uv;

        edge1 = pos2 - pos1;
        edge2 = pos3 - pos1;
        deltaUV1 = uv2 - uv1;
        deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);

        Vertices[Indices[indexIndex    ]].tangent = tangent;
        Vertices[Indices[indexIndex + 1]].tangent = tangent;
        Vertices[Indices[indexIndex + 2]].tangent = tangent;

        Vertices[Indices[indexIndex    ]].bitangent = bitangent;
        Vertices[Indices[indexIndex + 1]].bitangent = bitangent;
        Vertices[Indices[indexIndex + 2]].bitangent = bitangent;
    }
}
