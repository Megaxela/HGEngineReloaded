#include "Behaviours/Sprite.hpp"


RENDERING_BASE_MODULE_NS::Behaviours::Sprite::Sprite() :
    RenderBehaviour(Id),
    m_texture(nullptr),
    m_clipping()
{

}

RENDERING_BASE_MODULE_NS::Behaviours::Sprite::Sprite(RENDERING_BASE_MODULE_NS::Texture* texture) :
    RenderBehaviour(Id),
    m_texture(),
    m_clipping()
{
    setTexture(texture);
}

RENDERING_BASE_MODULE_NS::Behaviours::Sprite::~Sprite()
{

}

void RENDERING_BASE_MODULE_NS::Behaviours::Sprite::setTexture(RENDERING_BASE_MODULE_NS::Texture* texture)
{
    m_texture = texture;

    m_clipping.x = 0;
    m_clipping.y = 0;

    if (m_texture)
    {
        m_clipping.w = m_texture->size().x;
        m_clipping.h = m_texture->size().y;
    }
    else
    {
        m_clipping.w = 0;
        m_clipping.h = 0;
    }
}

RENDERING_BASE_MODULE_NS::Texture* RENDERING_BASE_MODULE_NS::Behaviours::Sprite::texture() const
{
    return m_texture;
}

void RENDERING_BASE_MODULE_NS::Behaviours::Sprite::setClipping(const ::UTILS_MODULE_NS::Rect &rect)
{
    m_clipping = rect;
}

::UTILS_MODULE_NS::Rect RENDERING_BASE_MODULE_NS::Behaviours::Sprite::clipping() const
{
    return m_clipping;
}
