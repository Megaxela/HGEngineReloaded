#pragma once

#include <Behaviour.hpp>
#include <Application.hpp>
#include <PhysicsController.hpp>
#include "TiledMapRenderer.hpp"

namespace HG::Standard::Behaviours
{
    /**
     * @brief Class, that describes behaviour, that
     * generates collision info based on tiled map renderer.
     * (It requires renderer, instead of tiled map, because
     * renderer defines some information about pixel size
     * and etc.)
     *
     * This behaviour detects layers with custom field `type` with
     * `Colliders` value or objects with `Collider` type.
     */
    template<typename Creator>
    class TiledMapCollider : public HG::Core::Behaviour
    {
    public:

        using ColliderRemover = std::function<void(void*, HG::Physics::Base::PhysicsController* physicsController)>;

        /**
         * @brief Initialization constructor.
         * @param renderer Pointer to tiled map renderer.
         * @param creator Callable object, that actually creates bodies.
         */
        explicit TiledMapCollider(ColliderRemover remover,
                                  TiledMapRenderer* renderer) :
            m_remover(remover),
            m_renderer(renderer),
            m_bodies()
        {

        }

        /**
         * @brief Destructor.
         */
        ~TiledMapCollider() override
        {
            clear();
        }

        /**
         * @brief Method for preparing colliders.
         */
        void prepare()
        {
            clear();

            proceedGroupLayer(m_renderer->map()->rootGroup(), {0, 0});
        }

        /**
         * @brief Method for removing bodies.
         */
        void clear()
        {
            for (auto&& body : m_bodies)
            {
                if (m_remover)
                {
                    m_remover(
                        body,
                        scene()
                            ->application()
                            ->template physicsController<HG::Physics::Base::PhysicsController>()
                    );
                }
            }
        }

    protected:

        void onStart() override
        {
            if (m_renderer)
            {
                prepare();
            }
        }

    private:

        void proceedGroupLayer(const HG::Standard::Behaviours::TiledMap::Group* group,
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

        void proceedObjectLayer(const HG::Standard::Behaviours::TiledMap::ObjectLayer* layer,
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
                        m_creator.createRectangle(
                            -(worldLayerSize.x / 2) + (offset.x + rectangle->position.x) * metersPerPixel,
                             (worldLayerSize.y / 2) - (offset.y + rectangle->position.y) * metersPerPixel,
                            rectangle->size.x * metersPerPixel,
                            rectangle->size.y * metersPerPixel,

                            scene()->application()->template physicsController<HG::Physics::Base::PhysicsController>()
                        )
                    );
                }
                else if (object->objectType == TiledMap::ObjectLayer::Object::Type::Ellipse)
                {
                    auto ellipse = static_cast<TiledMap::ObjectLayer::Ellipse*>(object);

                    m_bodies.push_back(
                        m_creator.createEllipse(
                            -(worldLayerSize.x / 2) + (offset.x + ellipse->position.x) * metersPerPixel,
                             (worldLayerSize.y / 2) - (offset.y + ellipse->position.y) * metersPerPixel,
                            ellipse->size.x * metersPerPixel,
                            ellipse->size.y * metersPerPixel,

                            scene()->application()->template physicsController<HG::Physics::Base::PhysicsController>()
                        )
                    );
                }
                else if (object->objectType == TiledMap::ObjectLayer::Object::Type::Polygon)
                {
                    auto polygon = static_cast<TiledMap::ObjectLayer::Polygon*>(object);

                    // todo: Optimize vector creation
                    std::vector<glm::vec2> globalPointsOffset;
                    globalPointsOffset.reserve(polygon->points.size());

                    for (auto&& point : polygon->points)
                    {
                        globalPointsOffset.emplace_back(
                            point.x * metersPerPixel,
                            -point.y * metersPerPixel
                        );
                    }

                    m_bodies.push_back(
                        m_creator.createPolygon(
                            -(worldLayerSize.x / 2) + (offset.x + polygon->position.x) * metersPerPixel,
                             (worldLayerSize.y / 2) - (offset.y + polygon->position.y) * metersPerPixel,
                            globalPointsOffset,

                            scene()->application()->template physicsController<HG::Physics::Base::PhysicsController>()
                        )
                    );
                }
            }
        }

        TiledMapRenderer* m_renderer;

        Creator m_creator;
        ColliderRemover m_remover;

        std::vector<void*> m_bodies;
    };
}