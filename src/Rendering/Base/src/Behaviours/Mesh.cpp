#include "Behaviours/Mesh.hpp"

RENDERING_BASE_MODULE_NS::Behaviours::Mesh::Mesh() :
    RenderBehaviour(typeid(Mesh).hash_code()),
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
