#include "Behaviours/Sprite.hpp"


RENDERING_BASE_MODULE_NS::Behaviours::Sprite::Sprite() :
    RenderBehaviour(Id),
    m_texture(nullptr)
{

}

RENDERING_BASE_MODULE_NS::Behaviours::Sprite::~Sprite()
{

}

void RENDERING_BASE_MODULE_NS::Behaviours::Sprite::setTexture(RENDERING_BASE_MODULE_NS::Texture* texture)
{
    m_texture = texture;
}

RENDERING_BASE_MODULE_NS::Texture* RENDERING_BASE_MODULE_NS::Behaviours::Sprite::texture() const
{
    return m_texture;
}
