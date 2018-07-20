#pragma once

#include <glm/vec2.hpp>
#include <ResourceAccessor.hpp>
#include <vector>
#include <Behaviour.hpp>
#include <cstdint>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Tiled map loader system.
     * Support version 1.1.5
     */
    class TiledMap : public ::CORE_MODULE_NS::Behaviour
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
            TileLayer() :
                Layer(Type::Tile),
                size({0, 0})
            {}


            glm::ivec2 size;
            std::vector<uint32_t> tiles;

        };

        struct ObjectLayer : Layer
        {
            ObjectLayer() :
                Layer(Type::Object)
            {}


        };

        /**
         * @brief Method for loading map from TMX file.
         * @param data Pointer to data from TMX file.
         * @return Loading success
         */
        bool loadMap(CORE_MODULE_NS::ResourceAccessor::DataPtr data);

    private:

        // todo: Add commentary
        bool proceedRootNode(rapidxml::xml_node<>* node, Group* target);

        bool parseTileset(rapidxml::xml_node<>* node);



        glm::ivec2 m_tileSize;

        bool m_isInfinite;
        glm::ivec2 m_size; // in tiles

        // Root layers group
        std::vector<Tileset*> m_tilesets;
        Group m_root;
    };
}

