// C++ STL
#include <charconv>

// HG::Core
#include <HG/Core/Data.hpp>
#include <HG/Core/ResourceAccessor.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/TiledMap.hpp>

// HG::Utils
#include <HG/Utils/base64.hpp>
#include <HG/Utils/zlib.hpp>

// RapidXML
#include <rapidxml.hpp>

// ALogger
#include <CurrentLogger.hpp>

namespace
{
const char* SUPPORTED_VERSION = "1.1.6";
}

HG::Standard::Behaviours::TiledMap::TiledMap() : m_properties(), m_tilesets(), m_animatedTiles(), m_root()
{
}

HG::Standard::Behaviours::TiledMap::~TiledMap()
{
    clear();
}

const std::vector<HG::Standard::Behaviours::TiledMap::Tileset*>& HG::Standard::Behaviours::TiledMap::tilesets() const
{
    return m_tilesets;
}

const HG::Standard::Behaviours::TiledMap::Group* HG::Standard::Behaviours::TiledMap::rootGroup() const
{
    return &m_root;
}

bool HG::Standard::Behaviours::TiledMap::loadMap(HG::Core::DataPtr data)
{
    // Parsing received data
    rapidxml::xml_document<> doc{};

    // RapidXML requires data copy to edit it
    std::string copy(reinterpret_cast<const char*>(data->data()), data->size());

    // 0 - no flags
    doc.parse<0>(const_cast<char*>(copy.c_str()));

    auto* mapNode = doc.first_node("map");

    if (mapNode == nullptr)
    {
        Error() << "Can't find root <map> tag.";
        return false;
    }

    m_properties.mapPath = data->id();

    rapidxml::xml_attribute<>* attribute;

    // Parsing map metadata
    if (!(attribute = mapNode->first_attribute("version")))
    {
        return false;
    }

    m_properties.version = attribute->value();

    if (!(attribute = mapNode->first_attribute("tiledversion")))
    {
        return false;
    }

    m_properties.tiledVersion = attribute->value();

    if (m_properties.tiledVersion != SUPPORTED_VERSION)
    {
        Warning() << "Tiled map parser supports " << SUPPORTED_VERSION << " tiled maps, trying to load "
                  << m_properties.tiledVersion;
    }

    if (!(attribute = mapNode->first_attribute("orientation")))
    {
        return false;
    }

    std::string_view sv(attribute->value(), attribute->value_size());

    if (sv == "orthogonal")
    {
        m_properties.orientation = Orientation::Orthogonal;
    }
    else
    {
        Warning() << sv << " orientation is not supported.";
    }

    if (!(attribute = mapNode->first_attribute("renderorder")))
    {
        return false;
    }

    sv = std::string_view(attribute->value(), attribute->value_size());

    if (sv == "right-down")
    {
        m_properties.renderOrder = TileRenderOrder::RightDown;
    }
    else
    {
        Warning() << sv << " render order is not supported.";
    }

    if (!(attribute = mapNode->first_attribute("width")))
    {
        return false;
    }

    m_properties.size.x = std::atoi(attribute->value());

    if (!(attribute = mapNode->first_attribute("height")))
    {
        return false;
    }

    m_properties.size.y = std::atoi(attribute->value());

    if (!(attribute = mapNode->first_attribute("tilewidth")))
    {
        return false;
    }

    m_properties.tileSize.x = std::atoi(attribute->value());

    if (!(attribute = mapNode->first_attribute("tileheight")))
    {
        return false;
    }

    m_properties.tileSize.y = std::atoi(attribute->value());

    return proceedRootNode(mapNode, &m_root);
}

void HG::Standard::Behaviours::TiledMap::clear()
{
    clearGroup(&m_root);

    for (auto&& tileset : m_tilesets)
    {
        delete tileset;
    }

    m_tilesets.clear();

    m_properties = MapProperties();
}

const HG::Standard::Behaviours::TiledMap::MapProperties& HG::Standard::Behaviours::TiledMap::properties() const
{
    return m_properties;
}

const HG::Standard::Behaviours::TiledMap::AnimatedTiles& HG::Standard::Behaviours::TiledMap::animatedTiles() const
{
    return m_animatedTiles;
}

void HG::Standard::Behaviours::TiledMap::clearGroup(HG::Standard::Behaviours::TiledMap::Group* grp)
{
    for (auto&& layer : grp->layers)
    {
        switch (layer->type)
        {
        case Layer::Type::Group:
            clearGroup(static_cast<Group*>(layer));
            break;
        case Layer::Type::Object: {
            auto objectLayer = static_cast<ObjectLayer*>(layer);

            for (auto&& object : objectLayer->objects)
            {
                delete object;
            }

            break;
        }
        case Layer::Type::Tile:
            break;
        case Layer::Type::Image:
            break;
        }

        delete layer;
    }

    grp->layers.clear();
}

bool HG::Standard::Behaviours::TiledMap::proceedRootNode(rapidxml::xml_node<>* root,
                                                         Group* target,
                                                         bool proceedLikeLayer)
{
    if (proceedLikeLayer)
    {
        if (!proceedBaseLayer(root, target))
        {
            return false;
        }

        rapidxml::xml_attribute<>* attribute = nullptr;

        if ((attribute = root->first_attribute("offsetx")))
        {
            target->offset.x = std::atoi(attribute->value());
        }

        if ((attribute = root->first_attribute("offsety")))
        {
            target->offset.y = std::atoi(attribute->value());
        }
    }

    for (auto* node = root->first_node(); node; node = node->next_sibling())
    {
        std::string_view name(node->name());

        if (name == "tileset")
        {
            if (!parseTileset(node))
            {
                return false;
            }
        }
        else if (name == "group")
        {
            auto newGroup = new Group();

            if (!proceedRootNode(node, newGroup, true))
            {
                return false;
            }

            target->layers.push_back(newGroup);
        }
        else if (name == "objectgroup")
        {
            if (!proceedObjectGroupNode(node, target))
            {
                return false;
            }
        }
        else if (name == "imagelayer")
        {
            Warning() << "Image layer is not implemented yet";
        }
        else if (name == "layer")
        {
            if (!proceedTileLayer(node, target))
            {
                return false;
            }
        }
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::parseTileset(rapidxml::xml_node<>* node)
{
    auto* image = node->first_node("image");

    // If <image> tag does not exists
    if (!image)
    {
        return false;
    }

    auto* tileset = new Tileset();

    rapidxml::xml_attribute<>* attribute = nullptr;

    { // First gID
        if (!(attribute = node->first_attribute("firstgid")))
        {
            delete tileset;
            return false;
        }

        tileset->firstGID = std::atoi(attribute->value());

        if (tileset->firstGID == 0)
        {
            delete tileset;
            return false;
        }
    }

    { // Name
        if (!(attribute = node->first_attribute("name")))
        {
            delete tileset;
            return false;
        }

        tileset->name = std::string(attribute->value(), attribute->value_size());
    }

    { // Width
        if (!(attribute = node->first_attribute("tilewidth")))
        {
            delete tileset;
            return false;
        }

        tileset->tileSize.x = std::atoi(attribute->value());
    }

    { // Height
        if (!(attribute = node->first_attribute("tileheight")))
        {
            delete tileset;
            return false;
        }

        tileset->tileSize.y = std::atoi(attribute->value());
    }

    { // Spacing
        if ((attribute = node->first_attribute("spacing")))
        {
            tileset->spacing = std::atoi(attribute->value());
        }
    }

    { // Tile Count
        if (!(attribute = node->first_attribute("tilecount")))
        {
            delete tileset;
            return false;
        }

        tileset->tileCount = std::atoi(attribute->value());
    }

    { // Columns
        if (!(attribute = node->first_attribute("columns")))
        {
            delete tileset;
            return false;
        }

        tileset->columns = std::atoi(attribute->value());
    }

    { // Image source
        if (!(attribute = image->first_attribute("source")))
        {
            delete tileset;
            return false;
        }

        tileset->path = std::string(attribute->value(), attribute->value_size());
    }

    {
        if (!(attribute = image->first_attribute("width")))
        {
            delete tileset;
            return false;
        }

        tileset->imageSize.x = std::atoi(attribute->value());
    }

    {
        if (!(attribute = image->first_attribute("height")))
        {
            delete tileset;
            return false;
        }

        tileset->imageSize.y = std::atoi(attribute->value());
    }

    for (auto tileNode = node->first_node("tile"); tileNode; tileNode = tileNode->next_sibling("tile"))
    {
        proceedTileNode(tileNode);
    }

    m_tilesets.push_back(tileset);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedTileNode(rapidxml::xml_node<>* node)
{
    rapidxml::xml_attribute<>* attribute = nullptr;

    uint32_t id = 0;

    { // First gID
        if (!(attribute = node->first_attribute("id")))
        {
            return false;
        }

        id = std::atoi(attribute->value());

        if (id == 0)
        {
            return false;
        }
    }

    for (auto childNode = node->first_node(); childNode; childNode = childNode->next_sibling())
    {
        std::string_view sv(childNode->name(), childNode->name_size());

        if (sv == "animation")
        {
            if (!proceedAnimation(childNode, id))
            {
                return false;
            }
        }
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedAnimation(rapidxml::xml_node<>* node, uint32_t tileId)
{
    TileAnimation animation;

    rapidxml::xml_attribute<>* attribute = nullptr;

    for (auto frameNode = node->first_node("frame"); frameNode; frameNode = frameNode->next_sibling("frame"))
    {
        TileAnimation::Frame frame;

        if (!(attribute = frameNode->first_attribute("tileid")))
        {
            return false;
        }

        frame.tile = std::atoi(attribute->value());

        if (!(attribute = frameNode->first_attribute("duration")))
        {
            return false;
        }

        frame.duration = std::chrono::milliseconds(std::atoi(attribute->value()));

        animation.frames.emplace_back(frame);
    }

    m_animatedTiles[tileId] = std::move(animation);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedObjectGroupNode(rapidxml::xml_node<>* node,
                                                                HG::Standard::Behaviours::TiledMap::Group* target)
{
    auto* objectGroup = new ObjectLayer();

    // Loading properties and name
    if (!proceedBaseLayer(node, objectGroup))
    {
        delete objectGroup;
        return false;
    }

    rapidxml::xml_attribute<>* attribute = nullptr;

    // Color
    if ((attribute = node->first_attribute("color")))
    {
        objectGroup->color = HG::Utils::Color::fromHex(attribute->value(), attribute->value_size());
    }

    // Iterating through objects
    for (auto* objectNode = node->first_node("object"); objectNode; objectNode = objectNode->next_sibling())
    {
        if (!proceedObjectNode(objectNode, objectGroup))
        {
            delete objectGroup;
            return false;
        }
    }

    target->layers.push_back(objectGroup);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedObjectNode(rapidxml::xml_node<>* node,
                                                           HG::Standard::Behaviours::TiledMap::ObjectLayer* target)
{
    // Detecting type

    // It's point object
    if (node->first_node("point"))
    {
        return proceedPointObject(node, target);
    }
    // It's ellipse object
    else if (node->first_node("ellipse"))
    {
        return proceedEllipseObject(node, target);
    }
    // It's polygon object
    else if (node->first_node("polygon"))
    {
        return proceedPolygonObject(node, target);
    }
    // It's text
    else if (node->first_node("text"))
    {
        return proceedTextObject(node, target);
    }
    // It's tile object
    else if (node->first_attribute("gid"))
    {
        return proceedTileObject(node, target);
    }
    // It's rectangle
    else
    {
        return proceedRectangleObject(node, target);
    }
}

bool HG::Standard::Behaviours::TiledMap::proceedProperties(rapidxml::xml_node<>* node,
                                                           HG::Standard::Behaviours::TiledMap::Properties& properties)
{
    for (auto propertyNode = node->first_node("property"); propertyNode; propertyNode = propertyNode->next_sibling())
    {
        auto* valueAttribute = propertyNode->first_attribute("value");

        if (!valueAttribute)
        {
            return false;
        }

        auto* nameAttribute = propertyNode->first_attribute("name");

        if (!nameAttribute)
        {
            return false;
        }

        auto* typeAttribute = propertyNode->first_attribute("type");

        std::string value(valueAttribute->value(), valueAttribute->value_size());
        std::string name(nameAttribute->value(), nameAttribute->value_size());

        // It's string
        if (!typeAttribute)
        {
            properties.emplace_back(name, Property(value, Property::Type::String));
            continue;
        }

        std::string_view type(typeAttribute->value(), typeAttribute->value_size());

        if (type == "bool")
        {
            properties.emplace_back(name, value == "true");
        }
        else if (type == "int")
        {
            properties.emplace_back(name, std::atoi(value.c_str()));
        }
        else if (type == "color")
        {
            properties.emplace_back(name, HG::Utils::Color::fromHex(value));
        }
        else if (type == "file")
        {
            properties.emplace_back(name, Property(value, Property::Type::File));
        }
        else if (type == "float")
        {
            properties.emplace_back(name, (float)std::atof(value.c_str()));
        }
        else
        {
            Warning() << "Found unknown type \"" << type << "\".";
        }
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedBaseLayer(rapidxml::xml_node<>* node,
                                                          HG::Standard::Behaviours::TiledMap::Layer* layer)
{
    rapidxml::xml_node<>* childNode           = nullptr;
    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if ((childNode = node->first_node("properties")))
    {
        if (!proceedProperties(childNode, layer->properties))
        {
            return false;
        }
    }

    if (!(childAttribute = node->first_attribute("name")))
    {
        return false;
    }

    layer->name = std::string(childAttribute->value(), childAttribute->value_size());

    if ((childAttribute = node->first_attribute("offsetx")))
    {
        layer->offset.x = std::atoi(childAttribute->value());
    }

    if ((childAttribute = node->first_attribute("offsety")))
    {
        layer->offset.y = std::atoi(childAttribute->value());
    }

    if ((childAttribute = node->first_attribute("opacity")))
    {
        layer->opacity = std::atof(childAttribute->value());
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedBaseObject(
    rapidxml::xml_node<>* node,
    HG::Standard::Behaviours::TiledMap::ObjectLayer::Object* object)
{
    rapidxml::xml_node<>* childNode           = nullptr;
    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if ((childNode = node->first_node("properties")))
    {
        if (!proceedProperties(childNode, object->properties))
        {
            return false;
        }
    }

    if ((childAttribute = node->first_attribute("name")))
    {
        object->name = std::string(childAttribute->value(), childAttribute->value_size());
    }

    {
        if (!(childAttribute = node->first_attribute("id")))
        {
            return false;
        }

        object->id = std::atoi(childAttribute->value());
    }

    {
        if (!(childAttribute = node->first_attribute("x")))
        {
            return false;
        }

        object->position.x = std::atof(childAttribute->value());
    }

    {
        if (!(childAttribute = node->first_attribute("y")))
        {
            return false;
        }

        object->position.y = std::atof(childAttribute->value());
    }

    {
        if ((childAttribute = node->first_attribute("type")))
        {
            object->type = std::string(childAttribute->value(), childAttribute->value_size());
        }
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedRectangleObject(rapidxml::xml_node<>* node,
                                                                HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto* rectangle = new ObjectLayer::Rectangle();

    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if (!proceedBaseObject(node, rectangle))
    {
        delete rectangle;
        return false;
    }

    if (!(childAttribute = node->first_attribute("width")))
    {
        delete rectangle;
        return false;
    }

    rectangle->size.x = std::atoi(childAttribute->value());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete rectangle;
        return false;
    }

    rectangle->size.y = std::atoi(childAttribute->value());

    layer->objects.push_back(rectangle);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedPointObject(rapidxml::xml_node<>* node,
                                                            HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto point = new ObjectLayer::Point();

    if (!proceedBaseObject(node, point))
    {
        delete point;
        return false;
    }

    layer->objects.push_back(point);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedEllipseObject(rapidxml::xml_node<>* node,
                                                              HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto ellipse = new ObjectLayer::Ellipse();

    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if (!proceedBaseObject(node, ellipse))
    {
        delete ellipse;
        return false;
    }

    if (!(childAttribute = node->first_attribute("width")))
    {
        delete ellipse;
        return false;
    }

    ellipse->size.x = std::atoi(childAttribute->value());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete ellipse;
        return false;
    }

    ellipse->size.y = std::atoi(childAttribute->value());

    layer->objects.push_back(ellipse);

    return true;
}

// todo: Remove this implementation if compiler will implement `std::from_chars` for floating.
namespace std
{
// Super stupid implementation, just to fit to API
from_chars_result from_chars(const char* begin, const char* end, float& f)
{
    from_chars_result error;

    error.ec = static_cast<errc>(0);

    f = std::atof(begin);

    auto iter = begin;

    if (*begin == '-' || *begin == '+')
    {
        ++iter;
    }

    bool pointFound = false;

    for (; iter < end; ++iter)
    {
        error.ptr = iter;

        if (!isdigit(*iter) && ((*iter) != '.' || pointFound))
        {
            break;
        }

        if ((*iter) == '.')
        {
            pointFound = true;
        }
    }

    error.ptr = iter;

    return error;
}
} // namespace std

bool HG::Standard::Behaviours::TiledMap::proceedPolygonObject(rapidxml::xml_node<>* node,
                                                              HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto polygon = new ObjectLayer::Polygon();

    rapidxml::xml_attribute<>* childAttribute = nullptr;
    rapidxml::xml_node<>* childNode           = nullptr;

    if (!proceedBaseObject(node, polygon))
    {
        delete polygon;
        return false;
    }

    if (!(childNode = node->first_node("polygon")))
    {
        delete polygon;
        return false;
    }

    if (!(childAttribute = childNode->first_attribute("points")))
    {
        delete polygon;
        return false;
    }

    // Points has format "0,0 1,1 2,2 3,3 4,4"
    auto pointsLength       = childAttribute->value_size();
    const auto* pointsBegin = childAttribute->value();
    const auto* pointsEnd   = childAttribute->value() + pointsLength;

    while (pointsBegin != pointsEnd)
    {
        glm::vec2 point;

        auto convResult = std::from_chars(pointsBegin, pointsEnd, point.x);

        if ((int)convResult.ec != 0)
        {
            delete polygon;
            return false;
        }

        pointsBegin = convResult.ptr;

        if (*pointsBegin != ',')
        {
            delete polygon;
            return false;
        }

        // Skipping ','
        ++pointsBegin;

        convResult = std::from_chars(pointsBegin, pointsEnd, point.y);

        if ((int)convResult.ec != 0)
        {
            delete polygon;
            return false;
        }

        pointsBegin = convResult.ptr;

        if (pointsBegin != pointsEnd)
        {
            if (*pointsBegin != ' ')
            {
                delete polygon;
                return false;
            }

            // Skipping space
            ++pointsBegin;
        }

        polygon->points.push_back(point);
    }

    layer->objects.push_back(polygon);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedTileObject(rapidxml::xml_node<>* node,
                                                           HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto tile = new ObjectLayer::Tile();

    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if (!proceedBaseObject(node, tile))
    {
        delete tile;
        return false;
    }

    if (!(childAttribute = node->first_attribute("gid")))
    {
        delete tile;
        return false;
    }

    tile->gid = static_cast<uint32_t>(std::atoi(childAttribute->value()));

    if (!(childAttribute = node->first_attribute("width")))
    {
        delete tile;
        return false;
    }

    tile->size.x = std::atoi(childAttribute->value());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete tile;
        return false;
    }

    tile->size.y = std::atoi(childAttribute->value());

    if ((childAttribute = node->first_attribute("rotation")))
    {
        tile->rotation = std::atoi(childAttribute->value());
    }

    layer->objects.push_back(tile);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedTextObject(rapidxml::xml_node<>* node,
                                                           HG::Standard::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto text = new ObjectLayer::Text();

    rapidxml::xml_node<>* textNode       = nullptr;
    rapidxml::xml_attribute<>* attribute = nullptr;

    if (!proceedBaseObject(node, text))
    {
        delete text;
        return false;
    }

    if (!(textNode = node->first_node("text")))
    {
        delete text;
        return false;
    }

    if (!(attribute = node->first_attribute("width")))
    {
        delete text;
        return false;
    }

    text->size.x = std::atof(attribute->value());

    if (!(attribute = node->first_attribute("height")))
    {
        delete text;
        return false;
    }

    text->size.y = std::atof(attribute->value());

    if (!(attribute = textNode->first_attribute("fontfamily")))
    {
        delete text;
        return false;
    }

    text->fontFamily = std::string(attribute->value(), attribute->value_size());

    if ((attribute = textNode->first_attribute("wrap")))
    {
        text->isWrappingEnabled = std::atoi(attribute->value()) == 1;
    }

    if ((attribute = textNode->first_attribute("color")))
    {
        text->color = HG::Utils::Color::fromHex(attribute->value(), attribute->value_size());
    }

    if ((attribute = textNode->first_attribute("bold")))
    {
        text->isBold = std::atoi(attribute->value()) == 1;
    }

    if ((attribute = textNode->first_attribute("italic")))
    {
        text->isItalic = std::atoi(attribute->value()) == 1;
    }

    if ((attribute = textNode->first_attribute("underline")))
    {
        text->isUnderline = std::atoi(attribute->value()) == 1;
    }

    if ((attribute = textNode->first_attribute("strikeout")))
    {
        text->isStrikeout = std::atoi(attribute->value()) == 1;
    }

    if ((attribute = textNode->first_attribute("halign")))
    {
        std::string_view sv(attribute->value(), attribute->value_size());

        if (sv == "center")
        {
            text->horizontalAlign = ObjectLayer::Text::HorizontalAlign::Center;
        }
        else if (sv == "left")
        {
            text->horizontalAlign = ObjectLayer::Text::HorizontalAlign::Left;
        }
        else if (sv == "right")
        {
            text->horizontalAlign = ObjectLayer::Text::HorizontalAlign::Right;
        }
        else
        {
            return false;
        }
    }

    if ((attribute = textNode->first_attribute("valign")))
    {
        std::string_view sv(attribute->value(), attribute->value_size());

        if (sv == "top")
        {
            text->verticalAlign = ObjectLayer::Text::VerticalAlign::Top;
        }
        else if (sv == "center")
        {
            text->verticalAlign = ObjectLayer::Text::VerticalAlign::Center;
        }
        else if (sv == "bottom")
        {
            text->verticalAlign = ObjectLayer::Text::VerticalAlign::Bottom;
        }
        else
        {
            return false;
        }
    }

    text->value = std::string(textNode->value(), textNode->value_size());

    layer->objects.push_back(text);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedTileLayer(rapidxml::xml_node<>* node,
                                                          HG::Standard::Behaviours::TiledMap::Group* target)
{
    auto tileLayer = new TileLayer();

    rapidxml::xml_node<>* childNode           = nullptr;
    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if (!proceedBaseLayer(node, tileLayer))
    {
        delete tileLayer;
        return false;
    }

    if (!(childNode = node->first_node("data")))
    {
        delete tileLayer;
        return false;
    }

    if (!(childAttribute = node->first_attribute("width")))
    {
        delete tileLayer;
        return false;
    }

    tileLayer->size.x = std::atoi(childAttribute->value());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete tileLayer;
        return false;
    }

    tileLayer->size.y = std::atoi(childAttribute->value());

    tileLayer->tiles.reserve(static_cast<std::size_t>(tileLayer->size.x * tileLayer->size.y));

    std::string_view encoding;
    std::string_view compression;

    if (!(childAttribute = childNode->first_attribute("encoding")))
    {
        delete tileLayer;
        return false;
    }

    encoding = childAttribute->value();

    if ((childAttribute = childNode->first_attribute("compression")))
    {
        compression = childAttribute->value();
    }

    // Truncate whitespaces
    auto dataValue = childNode->value();
    auto dataSize  = childNode->value_size();

    // Begin
    while (*dataValue && dataSize && (*dataValue == ' ' || *dataValue == '\n'))
    {
        ++dataValue;
        --dataSize;
    }

    // End
    auto iter = dataValue + dataSize - 1;

    while (iter > dataValue && (*dataValue == ' ' || *dataValue == '\n'))
    {
        --dataSize;
        --iter;
    }

    if (!performTileLayerDecoding(dataValue, dataSize, encoding, compression, tileLayer->tiles))
    {
        delete tileLayer;
        return false;
    }

    target->layers.push_back(tileLayer);

    return true;
}

bool HG::Standard::Behaviours::TiledMap::performTileLayerDecoding(const char* data,
                                                                  std::size_t dataSize,
                                                                  std::string_view encoding,
                                                                  std::string_view compression,
                                                                  std::vector<uint32_t>& result)
{
    if (encoding == "base64")
    {
        auto decoded = HG::Utils::Base64::Decode<char>(data, dataSize);

        if (compression == "zlib")
        {
            bool inflateResult = false;

            decoded = HG::Utils::ZLib::Inflate<char>(decoded, &inflateResult);

            if (!inflateResult)
            {
                return false;
            }
        }
        else if (compression == "gzip")
        {
            Error() << "GZIP compression is not supported yet.";
            return false;
        }

        auto castedData = (uint32_t*)decoded.data();

        for (std::size_t i = 0; i < decoded.size() / 4; ++i)
        {
            result.push_back(*(castedData++));
        }
    }
    else if (encoding == "csv")
    {
        return proceedCSVTileData(data, dataSize, result);
    }

    return true;
}

bool HG::Standard::Behaviours::TiledMap::proceedCSVTileData(const char* data,
                                                            std::size_t dataSize,
                                                            std::vector<uint32_t>& result)
{
    (void)data;
    (void)dataSize;
    (void)result;

    // todo: Add CSV support
    Warning() << "CSV does not supported yet.";
    return false;
}

HG::Standard::Behaviours::TiledMap::TileLayer::DecodedTile
HG::Standard::Behaviours::TiledMap::TileLayer::decodeTile(uint32_t tile)
{
    constexpr uint32_t IS_HORIZONTALLY_FLIPPED = 0x80000000;
    constexpr uint32_t IS_VERTICALLY_FLIPPED   = 0x40000000;
    constexpr uint32_t IS_DIAGONALLY_FLIPPED   = 0x20000000;

    DecodedTile decoded{};

    if (tile == 0)
    {
        return decoded;
    }

    decoded.horizontallyFlipped = static_cast<bool>(tile & IS_HORIZONTALLY_FLIPPED);
    decoded.verticallyFlipped   = static_cast<bool>(tile & IS_VERTICALLY_FLIPPED);
    decoded.diagonallyFlipped   = static_cast<bool>(tile & IS_DIAGONALLY_FLIPPED);
    decoded.actualTile          = tile & ~(IS_HORIZONTALLY_FLIPPED | IS_VERTICALLY_FLIPPED | IS_DIAGONALLY_FLIPPED);

    return decoded;
}
