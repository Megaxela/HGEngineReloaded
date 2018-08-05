#include <imgui.h>
#include "Behaviours/DebugControllerOverlay.hpp"

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::onUpdate()
{
    ImGui::Begin("GameObjects");

    m_gameObjects.clear();
    scene()->getGameObjects(m_gameObjects);

    proceedParentedGameObjects(nullptr);

    ImGui::End();
}

void STD_MODULE_NS::Behaviours::DebugControllerOverlay::proceedParentedGameObjects(::CORE_MODULE_NS::Transform *parent)
{
    for (auto&& gameObject : m_gameObjects)
    {
        if (gameObject->transform()->parent() == parent)
        {
            if (ImGui::TreeNode(gameObject->name().c_str()))
            {
                auto pos = gameObject->transform()->localPosition();
                auto rot = glm::eulerAngles(gameObject->transform()->localRotation());
                auto scale = gameObject->transform()->localScale();

                ImGui::InputFloat3("Position", reinterpret_cast<float *>(&pos));
                ImGui::InputFloat3("Rotation", reinterpret_cast<float *>(&rot));
                ImGui::InputFloat3("Scale",    reinterpret_cast<float *>(&scale));

                gameObject->transform()->setLocalPosition(pos);
                gameObject->transform()->setLocalRotation(rot);
                gameObject->transform()->setLocalScale(scale);

                // Displaying origin
                scene()->application()->renderer()->gizmos()->sphere(*(gameObject->transform()), 0.5f);

                if (gameObject->transform()->numberOfChildren() > 0)
                {
                    proceedParentedGameObjects(gameObject->transform());
                }

                ImGui::TreePop();
            }
        }
    }
}
