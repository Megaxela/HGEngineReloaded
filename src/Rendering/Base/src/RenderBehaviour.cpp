#include <GameObject.hpp>
#include "RenderBehaviour.hpp"

RENDERING_BASE_MODULE_NS::RenderBehaviour::RenderBehaviour(std::size_t type) :
    m_externalData(nullptr),
    m_type(type),
    m_parent(nullptr)
{

}

RENDERING_BASE_MODULE_NS::RenderBehaviour::~RenderBehaviour()
{
    clearExternalData();
}

void RENDERING_BASE_MODULE_NS::RenderBehaviour::clearExternalData()
{
    delete m_externalData;
}

std::size_t RENDERING_BASE_MODULE_NS::RenderBehaviour::behaviourType()
{
    return m_type;
}

::CORE_MODULE_NS::GameObject *RENDERING_BASE_MODULE_NS::RenderBehaviour::gameObject()
{
    return m_parent;
}

void RENDERING_BASE_MODULE_NS::RenderBehaviour::setParentGameObject(::CORE_MODULE_NS::GameObject *gameObject)
{
    m_parent = gameObject;
}
