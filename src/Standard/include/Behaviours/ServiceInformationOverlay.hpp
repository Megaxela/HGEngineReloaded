#pragma once

#include <Behaviour.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Behaviour for popup with service
     * information of render info.
     */
    class ServiceInformationOverlay : public ::CORE_MODULE_NS::Behaviour
    {
    protected:
        void onUpdate() override;

    private:

        void updateFrameGraph();

        const int m_framesCount = 240;
        float m_frames[240];
    };
}

