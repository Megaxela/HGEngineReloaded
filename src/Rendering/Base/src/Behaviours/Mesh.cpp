#include <memory>
#include "Behaviours/Mesh.hpp"

RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Mesh() :
    RenderBehaviour(Id),
    m_material(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Behaviours::Mesh::~Mesh()
{
    clearExternalData();
}

UTILS_MODULE_NS::MeshPtr RENDERING_BASE_MODULE_NS::Behaviours::Mesh::mesh() const
{
    return m_mesh;
}

void RENDERING_BASE_MODULE_NS::Behaviours::Mesh::setMesh(UTILS_MODULE_NS::MeshPtr mesh)
{
    m_mesh = std::move(mesh);
}

RENDERING_BASE_MODULE_NS::Material *RENDERING_BASE_MODULE_NS::Behaviours::Mesh::material() const
{
    return m_material;
}

void RENDERING_BASE_MODULE_NS::Behaviours::Mesh::setMaterial(RENDERING_BASE_MODULE_NS::Material *material)
{
    m_material = material;
}
