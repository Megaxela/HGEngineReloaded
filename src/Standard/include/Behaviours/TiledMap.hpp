#pragma once

#include <vector>
#include <variant>
#include <cstdint>
#include <glm/vec2.hpp>
#include <ResourceAccessor.hpp>
#include <Behaviour.hpp>
#include <Color.hpp>

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

        struct Property
        {
            enum class Type
            {
                Boolean,
                Color,
                File,
                Float,
                Integer,
                String
            };

            using ValueType = std::variant<
                bool,
                HG::Utils::Color,
                std::string,
                float,
                int
            >;

            explicit Property(bool boolean) :
                value(boolean),
                type(Type::Boolean)
            {}

            explicit Property(const HG::Utils::Color& color) :
                value(color),
                type(Type::Color)
            {}

            explicit Property(std::string val, Type type) :
                value(std::move(val)),
                type(type)
            {}

            explicit Property(float val) :
                value(val),
                type(Type::Float)
            {}

            explicit Property(int val) :
                value(val),
                type(Type::Integer)
            {}

            ValueType value;

            Type type;
        };

        using Properties = std::vector<
            std::pair<
                std::string,
                Property
            >
        >;

        /**
         * @brief Structure, that
         * describes tiled map.
         */
        struct Tileset
        {
            uint32_t firstGID;
            std::string name;
            glm::ivec2 tileSize;
            uint32_t spacing;
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
                type(t),
                properties()
            {}

            virtual ~Layer() = default;

            Type type;
            std::string name;
            Properties properties;
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

        /**
         * @brief Structure, that describes
         * object layer with objects.
         */
        struct ObjectLayer : Layer
        {
            /**
             * @brief Base object structure.
             */
            struct Object
            {
                enum class Type
                {
                    Rectangle,
                    Point,
                    Ellipse,
                    Polygon,
                    Tile,
                    Text
                };

                explicit Object(Type type) :
                    objectType(type),
                    id(0),
                    name(),
                    type(),
                    position(),
                    properties()
                {}

                virtual ~Object() = default;

                Type objectType;
                uint32_t id;
                std::string name;
                std::string type;
                glm::vec2 position;
                Properties properties;
            };

            /**
             * @brief Structure, that describes
             * rectangle object.
             */
            struct Rectangle : Object
            {
                Rectangle() :
                    Object(Type::Rectangle),
                    size({0, 0})
                {}

                glm::vec2 size;
            };

            /**
             * @brief Structure, that describes
             * point object.
             */
            struct Point : Object
            {
                Point() :
                    Object(Type::Point)
                {}
            };

            /**
             * @brief Structure, that describes
             * ellipse object.
             */
            struct Ellipse : Object
            {
                Ellipse() :
                    Object(Type::Ellipse),
                    size({0, 0})
                {}

                glm::vec2 size;
            };

            /**
             * @brief Structure, that describes
             * polygon object.
             */
            struct Polygon : Object
            {
                Polygon() :
                    Object(Type::Polygon)
                {}

                // This points are relative to `position` field.
                std::vector<
                    glm::vec2
                > points;
            };

            /**
             * @brief Structure, that describes
             * text object.
             */
            struct Text : Object
            {
                enum class VerticalAlign
                {
                    Top,
                    Center,
                    Bottom
                };

                enum class HorizontalAlign
                {
                    Left,
                    Center,
                    Right
                };

                Text() :
                    Object(Type::Text),
                    size({0, 0}),
                    fontFamily(),
                    color(),
                    isWrappingEnabled(false),
                    isBold(false),
                    isItalic(false),
                    isUnderline(false),
                    isStrikeout(false),
                    verticalAlign(VerticalAlign::Top),
                    horizontalAlign(HorizontalAlign::Left)
                {}

                glm::vec2 size;
                std::string fontFamily;
                HG::Utils::Color color;
                bool isWrappingEnabled;
                bool isBold;
                bool isItalic;
                bool isUnderline;
                bool isStrikeout;
                VerticalAlign verticalAlign;
                HorizontalAlign horizontalAlign;
            };

            /**
             * @brief Structure, that describes
             * single tile object.
             */
            struct Tile : Object
            {
                Tile() :
                    Object(Type::Tile),
                    gid(0),
                    size({0, 0}),
                    rotation(0.0f)
                {}

                uint32_t gid; // Unwrapped
                glm::vec2 size;
                float rotation; // Clockwise in degrees
            };

            ObjectLayer() :
                Layer(Type::Object)
            {}

            std::vector<Object*> objects;
            HG::Utils::Color color;
        };

        /**
         * @brief Method for loading map from TMX file.
         * @param data Pointer to data from TMX file.
         * @return Loading success
         */
        bool loadMap(CORE_MODULE_NS::ResourceAccessor::DataPtr data);

        /**
         * @brief Method for clearing tiled map.
         */
        void clear();

    private:

        void clearGroup(Group* grp);

        bool parseTileset(rapidxml::xml_node<>* node);

        // todo: Add commentary
        bool proceedRootNode(rapidxml::xml_node<>* node, Group* target);

        // todo: Add commentary
        bool proceedObjectGroupNode(rapidxml::xml_node<>* node, Group* target);

        // todo: Add commentary
        bool proceedObjectNode(rapidxml::xml_node<>* node, ObjectLayer* target);

        // todo: Add commentary
        bool proceedTileLayer(rapidxml::xml_node<>* node, Group* target);

        // todo: Add commentary
        bool proceedBaseLayer(rapidxml::xml_node<>* node, Layer* layer);

        // todo: Add commentary
        bool proceedBaseObject(rapidxml::xml_node<>* node, ObjectLayer::Object* object);

        // todo: Add commentary
        bool proceedRectangleObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedPointObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedEllipseObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedPolygonObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedTileObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedTextObject(rapidxml::xml_node<>* node, ObjectLayer* layer);

        // todo: Add commentary
        bool proceedProperties(rapidxml::xml_node<>* node, Properties& properties);

        glm::ivec2 m_tileSize;

        bool m_isInfinite;
        glm::ivec2 m_size; // in tiles

        // Root layers group
        std::vector<Tileset*> m_tilesets;
        Group m_root;
    };
}

