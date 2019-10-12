#pragma once

// C++ STL
#include <unordered_map>

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderData.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

// GLM
#include <glm/vec2.hpp>

namespace HG::Rendering::Base
{
// Forward declaration
class Texture;

/**
 * @brief Class, that describes target for
 * rendering.
 */
class RenderTarget
    : public HG::Core::CachableResource<RenderData>
    , public HG::Rendering::Base::RenderData
{
public:
    static constexpr std::size_t DataId = HG::Utils::StringTools::hash("RenderData::RenderTarget");

    using TextureContainer = std::unordered_map<uint32_t, HG::Rendering::Base::Texture*>;

    /**
     * @brief Constructor.
     */
    explicit RenderTarget(glm::ivec2 size, bool isDefault = false);

    /**
     * @brief Method for setting size for this
     * render target.
     * @param size Size.
     */
    void setSize(glm::ivec2 size);

    /**
     * @brief Method for getting render target size.
     */
    [[nodiscard]] glm::ivec2 size() const;

    /**
     * @brief Method for setting texture for color texture.
     * @param texture Pointer to texture.
     */
    void setColorTexture(HG::Rendering::Base::Texture* texture, uint32_t index = 0);

    /**
     * @brief Method for getting texture for color texture.
     */
    [[nodiscard]] HG::Rendering::Base::Texture* colorTexture(uint32_t index) const;

    /**
     * @brief Method for removing color texture.
     * @param index Index.
     */
    void removeColorTexture(uint32_t index);

    /**
     * @brief Method for checking is it
     * default render target.
     */
    [[nodiscard]] bool isDefault() const;

    /**
     * @brief Method for getting begin iterator for color
     * texture.
     */
    TextureContainer::iterator colorTextureBegin();

    /**
     * @brief Method for getting end iterator for color
     * texture.
     */
    TextureContainer::iterator colorTextureEnd();

private:
    TextureContainer m_colorTexture;

    glm::ivec2 m_size;

    bool m_isDefault;
};
} // namespace HG::Rendering::Base
