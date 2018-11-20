#pragma once

// C++ STL
#include <vector>
#include <unordered_map>

// GLM
#include <glm/vec2.hpp>

namespace HG::Rendering::Base
{
    class Texture;

    /**
     * @brief Class, that describes data for blitting.
     * Textures, positions etc.
     */
    class BlitData
    {
    public:

        struct PointData
        {
            glm::ivec2 point;
            glm::ivec2 uvPixels;
        };

        struct DataContainer
        {
            std::vector<PointData> vertices;
            std::vector<uint32_t> indices;
        };

        using BlitContainer = std::unordered_map<
                HG::Rendering::Base::Texture*,
                DataContainer
        >;

        /**
         * @brief Constructor.
         */
        BlitData();

        /**
         * @brief Method, that prepares rectangular
         * blitting.
         * @param texture Pointer to texture.
         * @param tl Top left texture UV.
         * @param br Bottom right texture UV.
         * @param pos Result position in pixels.
         * @param size Result size (if {0, 0} - size will
         * be taken from tl,br)
         */
        void blitRectangular(HG::Rendering::Base::Texture* texture,
                             glm::ivec2 tl, glm::ivec2 br,
                             glm::ivec2 pos, glm::ivec2 size={0, 0});

        /**
         * @brief Method, that prepares mesh blitting.
         * @param texture Pointer to texture.
         * @param points Vector of meshes.
         */
        void blitMesh(HG::Rendering::Base::Texture* texture,
                      const std::vector<PointData>& points,
                      const std::vector<uint32_t>& indices);

        /**
         * @brief Method for getting container with all blit
         * data.
         * @return Constant reference to blit container.
         */
        const BlitContainer& blitContainer() const;

    private:

        BlitContainer m_points;
    };
}
