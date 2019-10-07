#pragma once

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Utils
#include <HG/Utils/DoubleBufferContainer.hpp>

namespace HG::Rendering::Base
{
class RenderBehaviour;
}

namespace HG::Core
{
// Forward declaration
class Behaviour;
class Scene;
class Transform;

/**
 * @brief Class, that describes
 * abstract scene actor.
 * Behaviours, bound to gameobject will be
 * deleted if gameobject will be removed from scene.
 * (Because after removing gameobject from scene,
 * gameobject cached back to HG::Core::GameObjectCache,
 * and HG::Core::GameObjectCache::cache deletes all behaviours.
 * If it's not applicable - remove behaviours from gameobject
 * and behaviour's life owner will be changed to user.
 */
class GameObject : public HG::Core::CachableResource<GameObject>
{
public:
    /**
     * @brief Constructor.
     */
    GameObject();

    /**
     * @brief Destructor.
     */
    ~GameObject() override;

    // Disable copying (objects of this type has to be allocated in heap)
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    /**
     * @brief Method for updating gameobject
     * behaviours. Also this method is executing
     * `onStart` method if gameobject was recently
     * added.
     */
    void update();

    /**
     * @brief Method for getting parent scene.
     * @return Pointer to parent scene.
     */
    HG::Core::Scene* scene() const;

    /**
     * @brief Method for adding new behaviour.
     * @param behaviour New behaviour.
     */
    void addBehaviour(HG::Core::Behaviour* behaviour);

    /**
     * @brief Method for removing behaviour from
     * game object. If behaviour is deleting
     * this method will be called from `Behaviour` destructor.
     * If behaviour was not deleted it has to be
     * deleted manually, because GameObject `clear`
     * method fill not handle this behaviour anymore.
     * @param behaviour Pointer to behaviour.
     */
    void removeBehaviour(HG::Core::Behaviour* behaviour);

    /**
     * @brief Method for setting internal game object
     * name.
     * @param name Name.
     */
    void setName(std::string name);

    /**
     * @brief Method for getting game object
     * name.
     * @return String name.
     */
    std::string name() const;

    /**
     * @brief Method for checking is gameobject enabled.
     * If gameobject is disabled, no behaviours will be updated.
     * @return
     */
    bool isEnabled() const;

    /**
     * @brief Method for setting gameobject enabled state.
     * @param value Enabled state.
     */
    void setEnabled(bool value);

    /**
     * @brief Method for checking is gameobject hidden.
     * If gameobject is hidden - it will operate as
     * usual, but it will be impossible to locate
     * gameobject with external methods like HG::Core::Scene::findGameObject or
     * HG::Core::Scene::getGameObjects, but it's still possible to remove
     * or add gameobject to scene.
     */
    bool isHidden() const;

    /**
     * @brief Method for setting gameobject hidden state.
     * @return Hidden state.
     */
    void setHidden(bool value);

    /**
     * @brief Method for getting game object
     * transform.
     * @return Pointer to transform.
     */
    HG::Core::Transform* transform();

    /**
     * @brief Method for receiving
     * rendering behaviours of gameobject.
     * @tparam Container Container type.
     * @param container Container object.
     */
    template <typename Container>
    void getRenderingBehaviours(Container& container)
    {
        for (auto&& behaviour : m_renderBehaviours)
        {
            if (m_renderBehaviours.isRemoving(behaviour))
            {
                continue;
            }

            container.push_back((typename Container::value_type)behaviour);
        }
    }

    // todo: Add commentary
    template <typename BehaviourType>
    BehaviourType* findBehaviour()
    {
        for (auto&& behaviour : m_behaviours)
        {
            if (m_behaviours.isRemoving(behaviour))
            {
                continue;
            }

            auto casted = dynamic_cast<BehaviourType*>(behaviour);

            if (casted != nullptr)
            {
                return casted;
            }
        }

        if constexpr (std::is_base_of<HG::Rendering::Base::RenderBehaviour, BehaviourType>::value)
        {
            for (auto&& behaviour : m_renderBehaviours)
            {
                if (m_renderBehaviours.isRemoving(behaviour))
                {
                    continue;
                }

                auto casted = dynamic_cast<BehaviourType*>(behaviour);

                if (casted != nullptr)
                {
                    return casted;
                }
            }
        }

        return nullptr;
    }

    // todo: Add commentary
    template <typename BehaviourType, typename Container>
    void findBehaviours(Container& container)
    {
        for (auto&& behaviour : m_behaviours)
        {
            if (m_behaviours.isRemoving(behaviour))
            {
                continue;
            }

            auto casted = dynamic_cast<BehaviourType*>(behaviour);

            if (casted != nullptr)
            {
                container.push_back(behaviour);
            }
        }
    }

    // todo: Add commentary
    template <typename Container>
    void getBehaviours(Container& container)
    {
        for (auto&& behaviour : m_behaviours)
        {
            if (m_behaviours.isRemoving(behaviour))
            {
                continue;
            }

            container.push_back(behaviour);
        }
    }

protected:
    friend class Scene;

    /**
     * @brief Method for setting parent scene.
     * @param parent Parent scene.
     */
    void setParentScene(Scene* parent);

private:
    Transform* m_transform;

    HG::Utils::DoubleBufferContainer<HG::Core::Behaviour*> m_behaviours;

    HG::Utils::DoubleBufferContainer<HG::Rendering::Base::RenderBehaviour*> m_renderBehaviours;

    std::string m_name;

    HG::Core::Scene* m_parentScene;

    bool m_enabled;
    bool m_hidden;
};
} // namespace HG::Core
