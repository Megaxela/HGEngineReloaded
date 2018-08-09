#pragma once

#include <RenderBehaviour.hpp>
#include <Texture.hpp>
#include <Rect.hpp>

namespace HG::Rendering::Base::Behaviours
{
    /**
     * @brief Class, that describes sprite render behaviour.
     */
    class Sprite : public RenderBehaviour
    {
    public:

        static constexpr std::size_t Id = 0;

        /**
         * @brief Constructor.
         */
        Sprite();

        /**
         * @brief Initialize constructor.
         * @param texture Pointer to texture.
         */
        explicit Sprite(Texture* texture);

        /**
         * @brief Destructor.
         */
        ~Sprite() override;

        /**
         * @brief Method for setting texture for sprite.
         * @param texture Pointer to texture.
         */
        void setTexture(Texture* texture);

        /**
         * @brief Method for getting sprite texture.
         * @return Pointer to texture.
         */
        Texture* texture() const;

        /**
         * @brief Method for getting sprite clipping.
         * @param rect Rectangle.
         */
        void setClipping(const ::HG::Utils::Rect& rect);

        /**
         * @brief Method for getting sprite clipping.
         * @return Rectangle.
         */
        ::HG::Utils::Rect clipping() const;

    private:
        Texture* m_texture;
        ::HG::Utils::Rect m_clipping;
    };
}

