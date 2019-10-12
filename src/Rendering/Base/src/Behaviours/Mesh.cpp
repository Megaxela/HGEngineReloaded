// C++ STL
#include <memory>

// HG::Rendering::Base
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>

namespace HG::Rendering::Base::Behaviours
{
Mesh::Mesh(HG::Utils::MeshPtr mesh, HG::Rendering::Base::Material* material) :
    RenderBehaviour(RenderBehaviourId),
    m_mesh(std::move(mesh)),
    m_material(material)
{
}

HG::Utils::MeshPtr Mesh::mesh() const
{
    return m_mesh;
}

void Mesh::setMesh(HG::Utils::MeshPtr mesh)
{
    m_mesh = std::move(mesh);
}

HG::Rendering::Base::Material* Mesh::material() const
{
    return m_material;
}

void Mesh::setMaterial(HG::Rendering::Base::Material* material)
{
    m_material = material;
}
} // namespace HG::Rendering::Base::Behaviours
