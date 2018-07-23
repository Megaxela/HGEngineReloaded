#include <rapidxml.hpp>
#include "Behaviours/TiledMap.hpp"

bool STD_MODULE_NS::Behaviours::TiledMap::loadMap(CORE_MODULE_NS::ResourceAccessor::DataPtr data)
{
    // Parsing received data
    rapidxml::xml_document<> doc{};

    // 0 - no flags
    doc.parse<0>(
        const_cast<char*>(
            std::string(
                reinterpret_cast<const char*>(data->data()),
                data->size()
            ).c_str()
        )
    );

    auto* mapNode = doc.first_node("map");

    if (mapNode == nullptr)
    {
        Error() << "Can't find root <map> tag.";
        return false;
    }

    return proceedRootNode(mapNode, &m_root);
}

void STD_MODULE_NS::Behaviours::TiledMap::clear()
{
    clearGroup(&m_root);

    for (auto&& tileset : m_tilesets)
    {
        delete tileset;
    }

    m_tilesets.clear();

    m_tileSize = {0, 0};
    m_isInfinite = false;
    m_size = {0, 0};
}

void STD_MODULE_NS::Behaviours::TiledMap::clearGroup(STD_MODULE_NS::Behaviours::TiledMap::Group* grp)
{
    for (auto&& layer : grp->layers)
    {
        if (layer->type == Layer::Type::Group)
        {
            clearGroup(static_cast<Group*>(layer));
        }

        delete layer;
    }

    grp->layers.clear();
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedRootNode(rapidxml::xml_node<>* root, Group* target)
{
    for (auto* node = root->first_node(); node; node->next_sibling())
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

            if (!proceedRootNode(node, newGroup))
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

bool STD_MODULE_NS::Behaviours::TiledMap::parseTileset(rapidxml::xml_node<>* node)
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

        tileset->firstGID = static_cast<uint32_t>(
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str())
        );

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

        tileset->tileSize.x =
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str());
    }

    { // Height
        if (!(attribute = node->first_attribute("tileheight")))
        {
            delete tileset;
            return false;
        }

        tileset->tileSize.y =
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str());
    }

    { // Spacing
        if (!(attribute = node->first_attribute("spacing")))
        {
            delete tileset;
            return false;
        }

        tileset->spacing = static_cast<uint32_t>(
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str())
        );
    }

    { // Tile Count
        if (!(attribute = node->first_attribute("tilecount")))
        {
            delete tileset;
            return false;
        }

        tileset->spacing = static_cast<uint32_t>(
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str())
        );
    }

    { // Columns
        if (!(attribute = node->first_attribute("columns")))
        {
            delete tileset;
            return false;
        }

        tileset->spacing = static_cast<uint32_t>(
            std::atoi(std::string(attribute->value(), attribute->value_size()).c_str())
        );
    }

    { // Image source
        if (!(attribute = image->first_attribute("source")))
        {
            delete tileset;
            return false;
        }

        tileset->path = std::string(attribute->value(), attribute->value_size());
    }

    m_tilesets.push_back(tileset);

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedObjectGroupNode(rapidxml::xml_node<>* node,
                                                                 STD_MODULE_NS::Behaviours::TiledMap::Group* target)
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
        objectGroup->color = UTILS_MODULE_NS::Color::fromHex(
            attribute->value(),
            attribute->value_size()
        );
    }

    // Iterating through objects
    for (auto* objectNode = node->first_node("object");
         objectNode;
         objectNode->next_sibling())
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

bool STD_MODULE_NS::Behaviours::TiledMap::proceedObjectNode(rapidxml::xml_node<>* node,
                                                            STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* target)
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

bool STD_MODULE_NS::Behaviours::TiledMap::proceedProperties(rapidxml::xml_node<>* node,
                                                            STD_MODULE_NS::Behaviours::TiledMap::Properties& properties)
{
    for (auto propertyNode = node->first_node("property");
         propertyNode;
         propertyNode = node->next_sibling())
    {
        auto* typeAttribute = propertyNode->first_attribute("type");

        if (!typeAttribute)
        {
            return false;
        }

        auto* valueAttribute = propertyNode->first_attribute("value");

        if (!valueAttribute)
        {
            return false;
        }

        std::string_view type(typeAttribute->value(), typeAttribute->value_size());
        std::string value(valueAttribute->value(), valueAttribute->value_size());

        if (type == "bool")
        {
            properties.emplace_back(value == "true");
        }
        else if (type == "int")
        {
            properties.emplace_back(std::atoi(value.c_str()));
        }
        else if (type == "color")
        {
            properties.emplace_back(UTILS_MODULE_NS::Color::fromHex(value));
        }
        else if (type == "file")
        {
            properties.emplace_back(value, Property::Type::File);
        }
        else if (type == "float")
        {
            properties.emplace_back(std::atof(value.c_str()));
        }
        else // String
        {
            properties.emplace_back(value, Property::Type::String);
        }
    }

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedBaseLayer(rapidxml::xml_node<>* node,
                                                           STD_MODULE_NS::Behaviours::TiledMap::Layer* layer)
{
    rapidxml::xml_node<>* childNode = nullptr;
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

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedBaseObject(rapidxml::xml_node<>* node,
                                                            STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer::Object* object)
{
    rapidxml::xml_node<>* childNode = nullptr;
    rapidxml::xml_attribute<>* childAttribute = nullptr;

    if ((childNode = node->first_node("properties")))
    {
        if (!proceedProperties(childNode, object->properties))
        {
            return false;
        }
    }

    {
        if (!(childAttribute = node->first_attribute("name")))
        {
            return false;
        }

        object->name = std::string(childAttribute->value(), childAttribute->value_size());
    }

    {
        if (!(childAttribute = node->first_attribute("x")))
        {
            return false;
        }

        object->position.x = static_cast<uint32_t>(
            std::atoi(
                std::string(childAttribute->value(), childAttribute->value_size()).c_str()
            )
        );
    }

    {
        if (!(childAttribute = node->first_attribute("y")))
        {
            return false;
        }

        object->position.y = static_cast<uint32_t>(
            std::atoi(
                std::string(childAttribute->value(), childAttribute->value_size()).c_str()
            )
        );
    }

    {
        if ((childAttribute = node->first_attribute("type")))
        {
            object->type = std::string(childAttribute->value(), childAttribute->value_size());
        }
    }

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedRectangleObject(rapidxml::xml_node<>* node,
                                                                 STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
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

    rectangle->size.x =
        std::atoi(std::string(childAttribute->value(), childAttribute->value_size()).c_str());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete rectangle;
        return false;
    }

    rectangle->size.y =
        std::atoi(std::string(childAttribute->value(), childAttribute->value_size()).c_str());

    layer->objects.push_back(rectangle);

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedPointObject(rapidxml::xml_node<>* node,
                                                             STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
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

bool STD_MODULE_NS::Behaviours::TiledMap::proceedEllipseObject(rapidxml::xml_node<>* node,
                                                               STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
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

    ellipse->size.x =
        std::atoi(std::string(childAttribute->value(), childAttribute->value_size()).c_str());

    if (!(childAttribute = node->first_attribute("height")))
    {
        delete ellipse;
        return false;
    }

    ellipse->size.y =
        std::atoi(std::string(childAttribute->value(), childAttribute->value_size()).c_str());

    layer->objects.push_back(ellipse);

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedPolygonObject(rapidxml::xml_node<>* node,
                                                               STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
{
    auto polygon = new ObjectLayer::Polygon();

    rapidxml::xml_attribute<>* childAttribute = nullptr;
    rapidxml::xml_node<>* childNode = nullptr;

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
    

    layer->objects.push_back(polygon);

    return true;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedTileObject(rapidxml::xml_node<>* node,
                                                            STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
{
    return false;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedTextObject(rapidxml::xml_node<>* node,
                                                            STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer)
{
    return false;
}

bool STD_MODULE_NS::Behaviours::TiledMap::proceedTileLayer(rapidxml::xml_node<>* node,
                                                          STD_MODULE_NS::Behaviours::TiledMap::Group* target)
{
    
}
