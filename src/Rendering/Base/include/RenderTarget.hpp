#pragma once

// C++ STL
#include <map>

// HG::Rendering::Base
#include <RenderData.hpp>

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
    class RenderTarget : public HG::Rendering::Base::RenderData
    {
    public:

        static constexpr std::size_t DataId = 5;

        using TextureContainer = std::map<uint32_t, HG::Rendering::Base::Texture*>;

        /**
         * @brief Constructor.
         */
        explicit RenderTarget(glm::ivec2 size);

        /**
         * @brief Method for setting size for this
         * render target.
         * @param size Size.
         */
        void setSize(glm::ivec2 size);

        /**
         * @brief Method for getting render target size.
         */
        glm::ivec2 size() const;

        /**
         * @brief Method for setting texture for color texture.
         * @param texture Pointer to texture.
         */
        void setColorTexture(HG::Rendering::Base::Texture* texture, uint32_t index);

        /**
         * @brief Method for getting texture for color texture.
         */
        HG::Rendering::Base::Texture* colorTexture(uint32_t index) const;

        /**
         * @brief Method for removing color texture.
         * @param index Index.
         */
        void removeColorTexture(uint32_t index);

        /**
         * @brief Method for checking is it
         * default render target.
         */
        bool isDefault() const;

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

        friend class Renderer;

        /**
         * @brief Method only for Renderer for
         * creating default render target.
         * It's allocated with new.
         * @return Pointer to render target.
         */
        static RenderTarget* createDefault();

        TextureContainer m_colorTexture;

        glm::ivec2 m_size;

        bool m_isDefault;
    };
}

