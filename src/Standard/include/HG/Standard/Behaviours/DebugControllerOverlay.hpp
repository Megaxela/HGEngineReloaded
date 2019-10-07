#pragma once

// HG::Core
#include <HG/Core/Behaviour.hpp> // Required for inheritance
#include <HG/Core/GameObject.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

// GLM
#include <glm/vec2.hpp>

namespace HG::Core
{
class Transform;
class GameObject;
} // namespace HG::Core

namespace HG::Standard::Behaviours
{
/**
 * @brief Behaviour for controlling debug gameobjects
 * control.
 */
class DebugControllerOverlay : public HG::Core::Behaviour
{
public:
    template <typename B>
    typename std::enable_if<std::is_base_of<HG::Core::Behaviour, B>::value, DebugControllerOverlay*>::type
    addViewBehaviour(const std::string& name)
    {
        m_viewBehaviours.emplace_back(name,
                                      [](HG::Core::GameObject* gameObject) { return gameObject->findBehaviour<B>(); });

        return this;
    }

protected:
    void onUpdate() override;

private:
    void displayMenu();

    void displayGameObjectsWindow();

    void displayInspectorWindow();

    void proceedParentedGameObjects(HG::Core::Transform* parent);

    void proceedInspector();

    void displayPropertyWidget(const HG::Core::Behaviour::Property& property);

    std::vector<HG::Core::GameObject*> m_gameObjects;
    std::vector<HG::Core::Behaviour*> m_behaviours;
    std::vector<HG::Core::Behaviour::Property> m_properties;

    HG::Core::GameObject* m_activeGameObject = nullptr;

    std::vector<std::pair<std::string, std::function<HG::Core::Behaviour*(HG::Core::GameObject*)>>> m_viewBehaviours;

    HG_PROPERTY_DEFAULT(glm::vec2, OriginSize, glm::vec2(0.3f, 0.3f));
    HG_PROPERTY_DEFAULT(HG::Utils::Color, OriginColor, HG::Utils::Color::fromRGB(150, 180, 0));
};
} // namespace HG::Standard::Behaviours
