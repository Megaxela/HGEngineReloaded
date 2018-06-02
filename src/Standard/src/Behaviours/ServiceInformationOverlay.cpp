#include <imgui.h>
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

        ImGui::Text(
            "FPS: %f\n"
            "Timings:\n"
            "    Frame : %f ms =\n"
            "    Render: %f ms +\n"
            "    Update: %f ms\n",
            1000000.0 / timeStat->frameDeltaTime().count(),
            timeStat->frameDeltaTime ().count() / 1000.0f,
            timeStat->renderTime()     .count() / 1000.0f,
            timeStat->updateTime()     .count() / 1000.0f
        );

        // Moving all elements to left
        updateFrameGraph();

        ImGui::PlotLines(
            "",
            m_frames,
            m_framesCount,
            0,
            "FPS",
            0
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

    m_frames[m_framesCount - 1] =
        1000000.0f / scene()
                         ->application()
                         ->timeStatistics()
                         ->lastFrameDeltaTime()
                         .count();
}
