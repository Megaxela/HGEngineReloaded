#include <Controller.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "Behaviours/DebugControllerBehaviour.hpp"

PLAYRHO_PHYSICS_MODULE_NS::Behaviours::DebugControllerBehaviour::DebugControllerBehaviour()
{

}

void PLAYRHO_PHYSICS_MODULE_NS::Behaviours::DebugControllerBehaviour::onStart()
{
    setEnabled(false);
}

void PLAYRHO_PHYSICS_MODULE_NS::Behaviours::DebugControllerBehaviour::onUpdate()
{
    auto controller = scene()->application()->physicsController<Controller>();

    auto settings = controller->settings();

    if (ImGui::Begin("PlayRho settings"))
    {
        ImGui::Checkbox("Enabled Debug View", &settings->enabled);

        if (!settings->enabled)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        ImGui::InputFloat("Tolerance", &settings->tolerance);
        ImGui::Checkbox("Draw shapes", &settings->drawShapes);
        ImGui::Checkbox("Draw skins", &settings->drawSkins);
        ImGui::Checkbox("Draw joints", &settings->drawJoints);
        ImGui::Checkbox("Draw AABBs", &settings->drawAABBs);
        ImGui::Checkbox("Draw COMs", &settings->drawCOMs);

        if (!settings->enabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar(1);
        }

        ImGui::End();
    }
}
