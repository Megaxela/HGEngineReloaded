#include <memory>
#include "Behaviours/Mesh.hpp"

RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Mesh() :
    RenderBehaviour(Id),
    m_externalData(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Behaviours::Mesh::~Mesh()
{
    clearExternalData();
}

void RENDERING_BASE_MODULE_NS::Behaviours::Mesh::clearExternalData()
{
    delete m_externalData;
}

UTILS_MODULE_NS::MeshPtr RENDERING_BASE_MODULE_NS::Behaviours::Mesh::mesh() const
{
    return m_mesh;
}

void RENDERING_BASE_MODULE_NS::Behaviours::Mesh::setMesh(UTILS_MODULE_NS::MeshPtr mesh)
{
    m_mesh = std::move(mesh);
}
