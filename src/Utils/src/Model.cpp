// C++ STL
#include <algorithm>

// HG::Utils
#include <HG/Utils/Model.hpp>

HG::Utils::Model::Model(HG::Utils::Model* parent) :
    m_meshes(),
    m_parent(nullptr),
    m_children()
{
    setParent(parent);
}

HG::Utils::Model::~Model()
{
    for (auto&& child : m_children)
    {
        delete child;
    }
}

HG::Utils::Model* HG::Utils::Model::parent() const
{
    return m_parent;
}

void HG::Utils::Model::setParent(HG::Utils::Model* model)
{
    if (m_parent)
    {
        m_parent->m_children.erase(
            std::find(
                m_parent->m_children.begin(),
                m_parent->m_children.end(),
                this
            )
        );
    }

    m_parent = model;

    if (m_parent)
    {
        m_parent->m_children.push_back(this);
    }
}

const std::vector<HG::Utils::Model*>& HG::Utils::Model::children() const
{
    return m_children;
}

void HG::Utils::Model::addMesh(HG::Utils::MeshPtr mesh)
{
    m_meshes.push_back(mesh);
}

const std::vector<HG::Utils::MeshPtr>& HG::Utils::Model::meshes() const
{
    return m_meshes;
}
