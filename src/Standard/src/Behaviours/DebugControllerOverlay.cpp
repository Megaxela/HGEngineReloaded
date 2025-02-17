// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Transform.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/DebugControllerOverlay.hpp>
#include <HG/Standard/Behaviours/IngameConsole.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>
#include <HG/Utils/SystemTools.hpp>

// ImGui
#include <imgui.h>
#include <imgui_internal.h>

namespace HG::Standard::Behaviours
{
void DebugControllerOverlay::onUpdate()
{
    displayMenu();
    displayGameObjectsWindow();
    displayInspectorWindow();
}

void DebugControllerOverlay::displayMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Do something");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            for (auto&& viewElement : m_viewBehaviours)
            {
                auto behaviour = viewElement.second(gameObject());

                if (ImGui::MenuItem(viewElement.first.c_str(),
                                    nullptr,
                                    behaviour ? behaviour->isEnabled() : false,
                                    behaviour != nullptr))
                {
                    behaviour->setEnabled(!behaviour->isEnabled());
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void DebugControllerOverlay::displayGameObjectsWindow()
{
    ImGui::Begin("GameObjects");

    m_gameObjects.clear();
    scene()->getGameObjects(m_gameObjects);

    proceedParentedGameObjects(nullptr);

    ImGui::End();
}

void DebugControllerOverlay::displayInspectorWindow()
{
    static bool opened = true;

    if (m_activeGameObject)
    {
        opened = true;
        ImGui::Begin("Inspector", &opened);

        proceedInspector();

        ImGui::End();

        if (!opened)
        {
            m_activeGameObject = nullptr;
        }
    }
}

void DebugControllerOverlay::proceedInspector()
{
    // Display gameobject origin
    //    auto originPosition = m_activeGameObject->transform()->globalPosition();
    //    originPosition.z = 0.5f;
    //
    //    scene()->application()->renderer()->gizmos()->circle(
    //        originPosition,
    //        {m_propertyOriginSize.x, m_propertyOriginSize.y},
    //        m_propertyOriginColor
    //    );

    m_behaviours.clear();

    {
        bool gameObjectEnabled = m_activeGameObject->isEnabled();

        ImGui::Checkbox(m_activeGameObject->name().c_str(), &gameObjectEnabled);

        m_activeGameObject->setEnabled(gameObjectEnabled);
    }

    ImGui::Separator();

    auto pos   = m_activeGameObject->transform()->localPosition();
    auto rot   = glm::degrees(glm::eulerAngles(m_activeGameObject->transform()->localRotation()));
    auto scale = m_activeGameObject->transform()->localScale();

    ImGui::InputFloat3("Position", reinterpret_cast<float*>(&pos));
    ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&rot));
    ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale));

    m_activeGameObject->transform()->setLocalPosition(pos);
    m_activeGameObject->transform()->setLocalRotation(glm::radians(rot));
    m_activeGameObject->transform()->setLocalScale(scale);

    m_activeGameObject->getRenderingBehaviours(m_behaviours);
    m_activeGameObject->getBehaviours(m_behaviours);

    for (auto&& behaviour : m_behaviours)
    {
        ImGui::Separator();

        auto behaviourName = HG::Utils::SystemTools::getTypeName(*behaviour);

        bool enabled = behaviour->isEnabled();

        // Self skipping to prevent
        // debug controller self disabling
        if (behaviour == this)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::Checkbox(behaviourName.c_str(), &enabled);

        if (behaviour == this)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar(1);
        }

        behaviour->setEnabled(enabled);

        m_properties.clear();
        behaviour->getProperties(m_properties);

        for (auto&& property : m_properties)
        {
            displayPropertyWidget(property);
        }
    }
}

void DebugControllerOverlay::proceedParentedGameObjects(HG::Core::Transform* parent)
{
    HG::Core::GameObject* newActiveGameObject = nullptr;

    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->transform()->parent() == parent)
        {
            // Preparing flags
            auto hasChildren = !gameObject->transform()->children().empty();

            auto nodeFlags = (hasChildren ? 0U : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow |
                             (gameObject == m_activeGameObject ? ImGuiTreeNodeFlags_Selected : 0);

            bool opened = ImGui::TreeNodeEx(gameObject, nodeFlags, "%s", gameObject->name().c_str());

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

    if (newActiveGameObject != nullptr && newActiveGameObject != m_activeGameObject)
    {
        m_activeGameObject = newActiveGameObject;
    }
}

void DebugControllerOverlay::displayPropertyWidget(const HG::Core::Behaviour::Property& property)
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
    else if (property.typeInfo() == typeid(HG::Utils::Color))
    {
        auto color = property.getGetter<HG::Utils::Color>()();

        ImGui::ColorEdit3(property.name().c_str(), reinterpret_cast<float*>(&color));

        property.getSetter<HG::Utils::Color>()(color);
    }
    else if (property.typeInfo() == typeid(HG::Rendering::Base::Camera::Projection))
    {
        int proj = static_cast<int>(property.getGetter<HG::Rendering::Base::Camera::Projection>()());

        const char* items[] = {"Perspective", "Orthogonal"};

        ImGui::Combo(property.name().c_str(), &proj, items, 2);

        property.getSetter<HG::Rendering::Base::Camera::Projection>()(
            static_cast<HG::Rendering::Base::Camera::Projection>(proj));
    }
    else
    {
        ImGui::Text("Unknown property format: %s", property.type().c_str());
    }
}
} // namespace HG::Standard::Behaviours
