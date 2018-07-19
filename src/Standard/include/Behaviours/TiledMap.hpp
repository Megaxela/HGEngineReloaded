#pragma once

#include <glm/vec2.hpp>
#include <ResourceAccessor.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Tiled map loader system.
     * Support version 1.1.5
     */
    class TiledMap
    {
    public:

        // todo: Add commentary
        enum class Orientation
        {
              Orthogonal
            , Isometric
            , StaggeredIsometric
            , StaggeredHexagonal
        };

        // todo: Add commentary
        enum class TileLayerFormat
        {
              CSV
            , Base64
            , CompressedBase64
        };

        // todo: Add commentary
        enum class TileRenderOrder
        {
              RightDown
            , RightUp
            , LeftDown
            , LeftUp
        };

        /**
         * @brief Structure, that
         * describes tiled map.
         */
        struct Tileset
        {
            uint32_t firstGID;
            std::string name;
            uint32_t tileWidth;
            uint32_t tileHeight;
            uint32_t tileCount;
            uint32_t columns;

            std::string path;
        };

        /**
         * @brief Structure, that describes
         * base layer.
         */
        struct Layer
        {
            enum class Type
            {
                Tile,
                Object,
                Image,
                Group
            };

            explicit Layer(Type t) :
                type(t)
            {}

            Type type;
        };

        /**
         * @brief Structure, that describes
         * layer group.
         */
        struct Group : Layer
        {
            Group() :
                Layer(Type::Group),
                layers()
            {}

            std::vector<Layer*> layers;
        };

        /**
         * @brief Structure, that describes
         * tile layer with tile data.
         */
        struct TileLayer : Layer
        {
//            struct Tile
//            {
//                uint32_t gid;
//
//                inline bool isVerticallyFlipped()
//                {
//
//                }
//            };

            TileLayer() :
                Layer(Type::Tile),
                size({0, 0})
            {}

            glm::ivec2 size;

        };

        /**
         * @brief Method for loading map from TMX file.
         * @param data Pointer to data from TMX file.
         * @return Loading success
         */
        bool loadMap(CORE_MODULE_NS::ResourceAccessor::DataPtr data);



    private:

        glm::ivec2 m_tileSize;

        bool m_isInfinite;
        glm::ivec2 m_size; // in tiles

    };
}

