#pragma once

#include <Behaviour.hpp>

namespace HG::Standard::Behaviours
{
    /**
     * @brief Behaviour for popup with service
     * information of render info.
     */
    class ServiceInformationOverlay : public HG::Core::Behaviour
    {
    protected:
        void onUpdate() override;

    private:

        void updateFrameGraph();

        const int m_framesCount = 240;
        float m_frames[240];
    };
}

