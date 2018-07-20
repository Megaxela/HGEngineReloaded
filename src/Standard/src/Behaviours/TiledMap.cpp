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

        }

    }
}
