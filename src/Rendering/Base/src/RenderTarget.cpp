// HG::Rendering::Base
#include <HG/Rendering/Base/RenderTarget.hpp>

namespace HG::Rendering::Base
{
RenderTarget::RenderTarget(glm::ivec2 size, bool isDefault) :
    RenderData(DataId),
    m_colorTexture(),
    m_size(size),
    m_isDefault(isDefault)
{
}

void RenderTarget::setSize(glm::ivec2 size)
{
    m_size = size;
}

glm::ivec2 RenderTarget::size() const
{
    return m_size;
}

void RenderTarget::setColorTexture(Texture* texture, uint32_t index)
{
    m_colorTexture[index] = texture;
}

Texture* RenderTarget::colorTexture(uint32_t index) const
{
    return m_colorTexture.at(index);
}

void RenderTarget::removeColorTexture(uint32_t index)
{
    m_colorTexture.erase(index);
}

RenderTarget::TextureContainer::iterator RenderTarget::colorTextureBegin()
{
    return m_colorTexture.begin();
}

RenderTarget::TextureContainer::iterator RenderTarget::colorTextureEnd()
{
    return m_colorTexture.end();
}

bool RenderTarget::isDefault() const
{
    return m_isDefault;
}
} // namespace HG::Rendering::Base
