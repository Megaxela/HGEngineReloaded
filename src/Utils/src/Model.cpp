// C++ STL
#include <algorithm>

// HG::Utils
#include <HG/Utils/Model.hpp>

namespace HG::Utils
{
Model::Model(Model* parent) : m_meshes(), m_parent(nullptr), m_children()
{
    setParent(parent);
}

Model::~Model()
{
    for (auto&& child : m_children)
    {
        delete child;
    }
}

Model* Model::parent() const
{
    return m_parent;
}

void Model::setParent(Model* model)
{
    if (m_parent)
    {
        m_parent->m_children.erase(std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this));
    }

    m_parent = model;

    if (m_parent)
    {
        m_parent->m_children.push_back(this);
    }
}

const std::vector<Model*>& Model::children() const
{
    return m_children;
}

void Model::addMesh(MeshPtr mesh)
{
    m_meshes.push_back(mesh);
}

const std::vector<MeshPtr>& Model::meshes() const
{
    return m_meshes;
}
} // namespace HG::Utils
