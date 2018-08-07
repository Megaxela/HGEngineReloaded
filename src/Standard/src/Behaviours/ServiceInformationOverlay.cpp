#include <imgui.h>
#include <PhysicalResource.hpp>
#include "Behaviours/ServiceInformationOverlay.hpp"

void STD_MODULE_NS::Behaviours::ServiceInformationOverlay::onUpdate()
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

        auto totalRam = UTILS_MODULE_NS::PhysicalResource::getTotalRAM();

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
//        updateFrameGraph();
//
//        ImGui::PlotLines(
//            "",
//            m_frames,
//            m_framesCount,
//            0,
//            "Frame time",
//            0,
//            1000
//        );

        ImGui::Text(
            "RAM: %.1fMB / %.1fMB\n"
            "Used RAM: %.1fMB\n",
            (totalRam - UTILS_MODULE_NS::PhysicalResource::getFreeRAM()) / 1000.0f / 1000.0f,
            totalRam / 1000.0f / 1000.0f,
            UTILS_MODULE_NS::PhysicalResource::getProcessRAMUsed() / 1000.0f / 1000.0f
        );

        ImGui::End();
    }

}

void HG::Standard::Behaviours::ServiceInformationOverlay::updateFrameGraph()
{
    for (int i = 1; i < m_framesCount; ++i)
    {
        m_frames[i - 1] = m_frames[i];
    }

    m_frames[m_framesCount - 1] = scene()
                         ->application()
                         ->timeStatistics()
                         ->lastFrameDeltaTime()
                         .count() / 1000.0f;
}
