// HG::Rendering::Base
#include <Behaviours/Sprite.hpp>
#include <Texture.hpp>

HG::Rendering::Base::Behaviours::Sprite::Sprite() :
    RenderBehaviour(Id),
    m_texture(nullptr),
    m_clipping()
{

}

HG::Rendering::Base::Behaviours::Sprite::Sprite(HG::Rendering::Base::Texture* texture) :
    RenderBehaviour(Id),
    m_texture(),
    m_clipping()
{
    setTexture(texture);
}

HG::Rendering::Base::Behaviours::Sprite::~Sprite()
{

}

void HG::Rendering::Base::Behaviours::Sprite::setTexture(HG::Rendering::Base::Texture* texture)
{
    m_texture = texture;

    m_clipping.x = 0;
    m_clipping.y = 0;

    // todo: Due to new async resource loading system
    // it's irrelevant to setup clipping in setter.
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

HG::Rendering::Base::Texture* HG::Rendering::Base::Behaviours::Sprite::texture() const
{
    return m_texture;
}

void HG::Rendering::Base::Behaviours::Sprite::setClipping(const HG::Utils::Rect &rect)
{
    m_clipping = rect;
}

HG::Utils::Rect HG::Rendering::Base::Behaviours::Sprite::clipping() const
{
    return m_clipping;
}
