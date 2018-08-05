#pragma once

#include <Behaviour.hpp>
#include "TiledMapRenderer.hpp"

namespace STD_MODULE_NS::Behaviours
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
    class TiledMapCollider : public CORE_MODULE_NS::Behaviour
    {
    public:

        using ColliderCreator = std::function<void*(float x, float y, float w, float h, PHYSICS_BASE_MODULE_NS::PhysicsController* physicsController)>;

        using ColliderRemover = std::function<void(void*, PHYSICS_BASE_MODULE_NS::PhysicsController* physicsController)>;

        /**
         * @brief Initialization constructor.
         * @param renderer Pointer to tiled map renderer.
         * @param creator Callable object, that actually creates bodies.
         */
        explicit TiledMapCollider(TiledMapRenderer* renderer,
                                  ColliderCreator creator,
                                  ColliderRemover remover);

        /**
         * @brief Destructor.
         */
        ~TiledMapCollider() override;

        /**
         * @brief Method for preparing colliders.
         */
        void prepare();

        /**
         * @brief Method for removing bodies.
         */
        void clear();

    protected:

        void onStart() override;

    private:

        void proceedGroupLayer(const STD_MODULE_NS::Behaviours::TiledMap::Group* group,
                               glm::ivec2 offset);

        void proceedObjectLayer(const STD_MODULE_NS::Behaviours::TiledMap::ObjectLayer* layer,
                                glm::ivec2 offset);

        TiledMapRenderer* m_renderer;

        ColliderCreator m_creator;
        ColliderRemover m_remover;

        std::vector<void*> m_bodies;
    };
}


