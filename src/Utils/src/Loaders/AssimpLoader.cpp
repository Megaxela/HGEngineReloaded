// HG::Utils
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/Mesh.hpp>
#include <HG/Utils/Model.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace
{
HG::Utils::MeshPtr proceedMesh(aiMesh* mesh, const aiScene* scene)
{
    auto newMesh = std::make_shared<HG::Utils::Mesh>();

    for (std::size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        HG::Utils::Vertex vertex;

        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        // Is there any texture coords
        if (mesh->mTextureCoords[0])
        {
            vertex.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        else
        {
            vertex.uv = {0.0f, 0.0f};
        }

        if (mesh->mTangents)
        {
            vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
        }

        if (mesh->mBitangents)
        {
            vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
        }

        newMesh->Vertices.push_back(vertex);
    }

    for (std::size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        auto face = mesh->mFaces[i];

        for (std::size_t j = 0; j < face.mNumIndices; ++j)
        {
            newMesh->Indices.push_back(face.mIndices[j]);
        }
    }

    // todo: Add material processing here

    return newMesh;
}

/**
     * @brief Function for processing assimp node to target.
     * @param node Pointer to assimp node.
     * @param scene Pointer to assimp scene.
     * @param target Parsing target.
     */
void proceedNode(aiNode* node, const aiScene* scene, HG::Utils::Model* target)
{
    // Processing node meshes
    for (std::size_t i = 0; i < node->mNumMeshes; ++i)
    {
        auto assimpMesh = scene->mMeshes[node->mMeshes[i]];

        target->addMesh(proceedMesh(assimpMesh, scene));
    }

    // Processing children
    for (std::size_t i = 0; i < node->mNumChildren; ++i)
    {
        auto newModel = new HG::Utils::Model(target);

        proceedNode(node->mChildren[i], scene, newModel);
    }
}
} // namespace

namespace HG::Utils
{
AssimpLoader::AssimpLoader()
{
}

AssimpLoader::ResultType AssimpLoader::load(const std::byte* data, std::size_t size)
{
    Assimp::Importer importer;

    auto scene = importer.ReadFileFromMemory(data,
                                             size,
                                             aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
                                                 aiProcess_FlipWindingOrder,
                                             "obj");

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        HGError() << "Can't load model. Error: " << importer.GetErrorString();
        return nullptr;
    }

    // Creating root model
    auto rootModel = std::make_shared<Model>();

    proceedNode(scene->mRootNode, scene, rootModel.get());

    return rootModel;
}
} // namespace HG::Utils
