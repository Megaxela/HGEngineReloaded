// HG::Rendering::Base
#include <RenderTarget.hpp>

HG::Rendering::Base::RenderTarget::RenderTarget(glm::ivec2 size) :
    RenderData(DataId),
    m_colorTexture(),
    m_size(size),
    m_isDefault(false)
{

}

void HG::Rendering::Base::RenderTarget::setSize(glm::ivec2 size)
{
    m_size = size;
}

glm::ivec2 HG::Rendering::Base::RenderTarget::size() const
{
    return m_size;
}

void HG::Rendering::Base::RenderTarget::setColorTexture(HG::Rendering::Base::Texture* texture, uint32_t index)
{
    m_colorTexture[index] = texture;
}

HG::Rendering::Base::Texture* HG::Rendering::Base::RenderTarget::colorTexture(uint32_t index) const
{
    return m_colorTexture.at(index);
}

void HG::Rendering::Base::RenderTarget::removeColorTexture(uint32_t index)
{
    m_colorTexture.erase(index);
}

HG::Rendering::Base::RenderTarget::TextureContainer::iterator
HG::Rendering::Base::RenderTarget::colorTextureBegin()
{
    return m_colorTexture.begin();
}

HG::Rendering::Base::RenderTarget::TextureContainer::iterator
HG::Rendering::Base::RenderTarget::colorTextureEnd()
{
    return m_colorTexture.end();
}

bool HG::Rendering::Base::RenderTarget::isDefault() const
{
    return m_isDefault;
}

HG::Rendering::Base::RenderTarget* HG::Rendering::Base::RenderTarget::createDefault()
{
    auto target = new RenderTarget({0, 0});

    target->m_isDefault = true;

    return target;
}
