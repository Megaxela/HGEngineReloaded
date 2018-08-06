#include <imgui.h>
#include "Behaviours/DebugControllerOverlay.hpp"
#include <Color.hpp>

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::onUpdate()
{
    ImGui::Begin("GameObjects");

    m_gameObjects.clear();
    scene()->getGameObjects(m_gameObjects);

    proceedParentedGameObjects(nullptr);

    ImGui::End();

    ImGui::Begin("Inspector");

    if (m_activeGameObject)
    {
        proceedInspector();
    }

    ImGui::End();
}

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::proceedInspector()
{
    // Display gameobject origin
    auto originPosition = m_activeGameObject->transform()->globalPosition();
    originPosition.z = 0.5f;

    scene()->application()->renderer()->gizmos()->multiLine({
        originPosition + glm::vec3(-m_propertyOriginSize.x, -m_propertyOriginSize.y, originPosition.z),
        originPosition + glm::vec3( m_propertyOriginSize.x, -m_propertyOriginSize.y, originPosition.z),
        originPosition + glm::vec3( m_propertyOriginSize.x,  m_propertyOriginSize.y, originPosition.z),
        originPosition + glm::vec3(-m_propertyOriginSize.x,  m_propertyOriginSize.y, originPosition.z),
        originPosition + glm::vec3(-m_propertyOriginSize.x, -m_propertyOriginSize.y, originPosition.z)
    }, m_propertyOriginColor);

    m_behaviours.clear();

    {
        bool gameObjectEnabled = m_activeGameObject->isEnabled();

        ImGui::Checkbox(m_activeGameObject->name().c_str(), &gameObjectEnabled);

        m_activeGameObject->setEnabled(gameObjectEnabled);
    }


    ImGui::Separator();

    auto pos = m_activeGameObject->transform()->localPosition();
    auto rot = glm::degrees(glm::eulerAngles(m_activeGameObject->transform()->localRotation()));
    auto scale = m_activeGameObject->transform()->localScale();

    ImGui::InputFloat3("Position", reinterpret_cast<float *>(&pos));
    ImGui::InputFloat3("Rotation", reinterpret_cast<float *>(&rot));
    ImGui::InputFloat3("Scale",    reinterpret_cast<float *>(&scale));

    m_activeGameObject->transform()->setLocalPosition(pos);
    m_activeGameObject->transform()->setLocalRotation(glm::radians(rot));
    m_activeGameObject->transform()->setLocalScale(scale);

    m_activeGameObject->getBehaviours(m_behaviours);

    for (auto&& behaviour : m_behaviours)
    {
        ImGui::Separator();

        auto behaviourName = SystemTools::getTypeName(*behaviour);

        bool enabled = behaviour->isEnabled();

        ImGui::Checkbox(behaviourName.c_str(), &enabled);

        behaviour->setEnabled(enabled);

        m_properties.clear();
        behaviour->getProperties(m_properties);

        for (auto&& property : m_properties)
        {
            displayPropertyWidget(property);
        }
    }
}

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::proceedParentedGameObjects(::CORE_MODULE_NS::Transform *parent)
{
    ::CORE_MODULE_NS::GameObject* newActiveGameObject = nullptr;

    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->transform()->parent() == parent)
        {
            // Preparing flags
            auto hasChildren = gameObject->transform()->numberOfChildren() > 0;

            auto nodeFlags =
                (hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf) |
                ImGuiTreeNodeFlags_OpenOnArrow |
//                ImGuiTreeNodeFlags_OpenOnDoubleClick |
                (gameObject == m_activeGameObject ? ImGuiTreeNodeFlags_Selected : 0);

            bool opened = ImGui::TreeNodeEx(gameObject, nodeFlags, gameObject->name().c_str());

            if (ImGui::IsItemClicked())
            {
                newActiveGameObject = gameObject;
            }

            if (opened)
            {
                // Displaying origin

                if (hasChildren)
                {
                    proceedParentedGameObjects(gameObject->transform());
                }

                ImGui::TreePop();
            }
        }
    }

    if (newActiveGameObject != nullptr &&
        newActiveGameObject != m_activeGameObject)
    {
        m_activeGameObject = newActiveGameObject;
    }
}

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::displayPropertyWidget(const ::CORE_MODULE_NS::Behaviour::Property& property)
{
    if (property.typeInfo() == typeid(float))
    {
        auto v = property.getGetter<float>()();

        ImGui::InputFloat(property.name().c_str(), &v);

        property.getSetter<float>()(v);
    }
    else if (property.typeInfo() == typeid(glm::vec2))
    {
        auto v = property.getGetter<glm::vec2>()();

        ImGui::InputFloat2(property.name().c_str(), reinterpret_cast<float*>(&v));

        property.getSetter<glm::vec2>()(v);
    }
    else if (property.typeInfo() == typeid(::UTILS_MODULE_NS::Color))
    {
        auto color = property.getGetter<::UTILS_MODULE_NS::Color>()();

        ImGui::ColorEdit3(property.name().c_str(), reinterpret_cast<float*>(&color));

        property.getSetter<::UTILS_MODULE_NS::Color>()(color);
    }
    else
    {
        ImGui::Text("Unknown property format: %s", property.type().c_str());
    }
}
