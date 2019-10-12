// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/CountStatistics.hpp>
#include <HG/Core/ResourceManager.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/ThreadPool.hpp>
#include <HG/Core/TimeStatistics.hpp>

// HG::Standard
#include <HG/Standard/Behaviours/ServiceInformationOverlay.hpp>

// HG::Utils
#include <HG/Utils/PhysicalResource.hpp>

// ImGui
#include <imgui.h>

namespace HG::Standard::Behaviours
{
void ServiceInformationOverlay::onUpdate()
{
    const float DISTANCE = 10.0f;

    ImVec2 window_pos(DISTANCE, DISTANCE);
    ImVec2 window_pos_pivot(0.0f, 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

    if (ImGui::Begin("Service Information",
                     nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        auto timeStat  = scene()->application()->timeStatistics();
        auto countStat = scene()->application()->countStatistics();

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
            timeStat->renderTime().count() / 1000.0f,
            timeStat->updateTime().count() / 1000.0f,
            timeStat->physicsTime().count() / 1000.0f,
            timeStat->frameDeltaTime().count() / 1000.0f);

        // Resources

        ImGui::Text(
            "RAM: %.1fMB / %.1fMB\n"
            "Used RAM: %.1fMB\n",
            (totalRam - HG::Utils::PhysicalResource::getFreeRAM()) / 1000.0f / 1000.0f,
            totalRam / 1000.0f / 1000.0f,
            HG::Utils::PhysicalResource::getProcessRAMUsed() / 1000.0f / 1000.0f);

        ImGui::Text("Resource queue: %ld",
                    scene()->application()->threadPool()->numberOfJobs(HG::Core::ThreadPool::Type::FileLoadingThread));

        // Counters
        ImGui::Text("Vertices: %llu\n", countStat->value(HG::Core::CountStatistics::CommonCounter::NumberOfVertices));

        ImGui::End();
    }
}
} // namespace HG::Standard::Behaviours
