#include <memory>
#include <Behaviours/Mesh.hpp>

HG::Rendering::Base::Behaviours::Mesh::Mesh() :
    RenderBehaviour(Id),
    m_material(nullptr)
{

}

HG::Rendering::Base::Behaviours::Mesh::~Mesh()
{
    clearExternalData();
}

HG::Utils::MeshPtr HG::Rendering::Base::Behaviours::Mesh::mesh() const
{
    return m_mesh;
}

void HG::Rendering::Base::Behaviours::Mesh::setMesh(HG::Utils::MeshPtr mesh)
{
    m_mesh = std::move(mesh);
}

HG::Rendering::Base::Material *HG::Rendering::Base::Behaviours::Mesh::material() const
{
    return m_material;
}

void HG::Rendering::Base::Behaviours::Mesh::setMaterial(HG::Rendering::Base::Material *material)
{
    m_material = material;
}
