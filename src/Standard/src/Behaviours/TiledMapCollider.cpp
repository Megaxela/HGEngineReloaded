#include <utility>
#include <Behaviours/TiledMapCollider.hpp>


STD_MODULE_NS::Behaviours::TiledMapCollider::TiledMapCollider(STD_MODULE_NS::Behaviours::TiledMapRenderer *renderer,
                                                              STD_MODULE_NS::Behaviours::TiledMapCollider::ColliderCreator creator,
                                                              STD_MODULE_NS::Behaviours::TiledMapCollider::ColliderRemover remover) :
    m_renderer(renderer),
    m_creator(std::move(creator)),
    m_remover(std::move(remover)),
    m_bodies()
{

}

STD_MODULE_NS::Behaviours::TiledMapCollider::~TiledMapCollider()
{
    clear();
}

void STD_MODULE_NS::Behaviours::TiledMapCollider::prepare()
{
    clear();

    proceedGroupLayer(m_renderer->map()->rootGroup(), {0, 0});

    Info() << "Created " << m_bodies.size() << " bodies.";
}

void STD_MODULE_NS::Behaviours::TiledMapCollider::proceedGroupLayer(const STD_MODULE_NS::Behaviours::TiledMap::Group *group,
                                                                    glm::ivec2 offset)
{
    for (auto&& layer : group->layers)
    {
        switch (layer->type)
        {
        case TiledMap::Layer::Type::Object:
            proceedObjectLayer(
                static_cast<TiledMap::ObjectLayer*>(layer),
                offset + layer->offset
            );
            break;

        case TiledMap::Layer::Type::Group:
            proceedGroupLayer(
                static_cast<TiledMap::Group*>(layer),
                offset + layer->offset
            );
            break;

        case TiledMap::Layer::Type::Tile:  // fallthrough
        case TiledMap::Layer::Type::Image:
            break;
        }
    }
}

void STD_MODULE_NS::Behaviours::TiledMapCollider::proceedObjectLayer(const STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer *layer,
                                                                     glm::ivec2 offset)
{
    auto metersPerPixel = m_renderer->metersPerPixel();

    glm::vec2 worldMapTileSize = {
        m_renderer->map()->properties().tileSize.x * metersPerPixel,
        m_renderer->map()->properties().tileSize.y * metersPerPixel
    };

    glm::vec2 worldLayerSize = {
        m_renderer->map()->properties().size.x * worldMapTileSize.x,
        m_renderer->map()->properties().size.y * worldMapTileSize.y
    };

    for (auto&& object : layer->objects)
    {
        // todo: Change API to support multiple shapes
        if (object->objectType == TiledMap::ObjectLayer::Object::Type::Rectangle)
        {
            auto rectangle = static_cast<TiledMap::ObjectLayer::Rectangle*>(object);

            m_bodies.push_back(
                m_creator(
                    -(worldLayerSize.x / 2) + (offset.x + rectangle->position.x) * metersPerPixel,
                     (worldLayerSize.y / 2) - (offset.y + rectangle->position.y) * metersPerPixel,
                    rectangle->size.x * metersPerPixel,
                    rectangle->size.y * metersPerPixel,

                    scene()->application()->physicsController<PHYSICS_BASE_MODULE_NS::PhysicsController>()
                )
            );
        }
    }
}

void STD_MODULE_NS::Behaviours::TiledMapCollider::clear()
{
    for (auto&& body : m_bodies)
    {
        if (m_remover)
        {
            m_remover(body, scene()->application()->physicsController<PHYSICS_BASE_MODULE_NS::PhysicsController>());
        }
    }
}

void STD_MODULE_NS::Behaviours::TiledMapCollider::onStart()
{
    if (m_renderer)
    {
        prepare();
    }
}
