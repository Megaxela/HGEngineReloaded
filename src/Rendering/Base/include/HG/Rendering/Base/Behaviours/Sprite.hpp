#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderBehaviour.hpp> // Required for inheritance

// HG::Utils
#include <HG/Utils/Rect.hpp>
#include <HG/Utils/StringTools.hpp>

namespace HG::Rendering::Base
{
class Texture;
}

namespace HG::Rendering::Base::Behaviours
{
/**
 * @brief Class, that describes sprite render behaviour.
 */
class Sprite : public HG::Rendering::Base::RenderBehaviour
{
public:
    static constexpr std::size_t RenderBehaviourId = HG::Utils::StringTools::hash("RenderBehaviour::Sprite");

    /**
     * @brief Constructor.
     */
    Sprite();

    /**
     * @brief Initialize constructor.
     * @param texture Pointer to texture.
     */
    explicit Sprite(HG::Rendering::Base::Texture* texture);

    /**
     * @brief Destructor.
     */
    ~Sprite() override;

    /**
     * @brief Method for setting texture for sprite.
     * @param texture Pointer to texture.
     */
    void setTexture(HG::Rendering::Base::Texture* texture);

    /**
     * @brief Method for getting sprite texture.
     * @return Pointer to texture.
     */
    [[nodiscard]] HG::Rendering::Base::Texture* texture() const;

    /**
     * @brief Method for getting sprite clipping.
     * @param rect Rectangle.
     */
    void setClipping(const HG::Utils::Rect& rect);

    /**
     * @brief Method for getting sprite clipping.
     * @return Rectangle.
     */
    [[nodiscard]] HG::Utils::Rect clipping() const;

private:
    HG::Rendering::Base::Texture* m_texture;
    HG::Utils::Rect m_clipping;
    bool m_manualClipping;
};
} // namespace HG::Rendering::Base::Behaviours
