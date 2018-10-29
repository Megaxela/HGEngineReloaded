#pragma once

// HG::Core
#include <HG/Core/Behaviour.hpp> // Required for inheritance

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
    };
}

