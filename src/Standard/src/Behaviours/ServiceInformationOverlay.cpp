#include <Behaviours/ServiceInformationOverlay.hpp>
#include <PhysicalResource.hpp>
#include <Scene.hpp>
#include <Application.hpp>
#include <imgui.h>

void HG::Standard::Behaviours::ServiceInformationOverlay::onUpdate()
{
    const float DISTANCE = 10.0f;

    ImVec2 window_pos(DISTANCE, DISTANCE);
    ImVec2 window_pos_pivot(0.0f, 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

    if (ImGui::Begin(
        "Service Information",
        nullptr,
          ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav
    ))
    {
        auto timeStat = scene()->application()->timeStatistics();

        auto totalRam = HG::Utils::PhysicalResource::getTotalRAM();

        ImGui::Text(
            "FPS: %f\n"
            "Timings:\n"
            "    Render:  %f ms\n"
            "  + Update:  %f ms\n"
            "  + Physics: %f ms\n"
            "------------------------\n"
            "    Frame :  %f ms\n",
            1000000.0 / timeStat->frameDeltaTime().count(),
            timeStat->renderTime()     .count() / 1000.0f,
            timeStat->updateTime()     .count() / 1000.0f,
            timeStat->physicsTime()    .count() / 1000.0f,
            timeStat->frameDeltaTime() .count() / 1000.0f
        );

        // Moving all elements to left

        ImGui::Text(
            "RAM: %.1fMB / %.1fMB\n"
            "Used RAM: %.1fMB\n",
            (totalRam - HG::Utils::PhysicalResource::getFreeRAM()) / 1000.0f / 1000.0f,
            totalRam / 1000.0f / 1000.0f,
            HG::Utils::PhysicalResource::getProcessRAMUsed() / 1000.0f / 1000.0f
        );

        ImGui::End();
    }

}
