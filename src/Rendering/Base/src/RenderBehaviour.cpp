#include "RenderBehaviour.hpp"

RENDERING_BASE_MODULE_NS::RenderBehaviour::RenderBehaviour(std::size_t type) :
    m_type(type)
{

}

std::size_t RENDERING_BASE_MODULE_NS::RenderBehaviour::behaviourType()
{
    return m_type;
}
