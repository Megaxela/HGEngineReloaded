#pragma once

#include <Behaviour.hpp>

namespace STD_MODULE_NS::Behaviours
{
    /**
     * @brief Behaviour for popup with service 
     * information of render info.
     */
    class ServiceInformationPopup : public ::CORE_MODULE_NS::Behaviour
    {
    public:
        
        /**
         * @brief Default constructor.
         */
        ServiceInformationPopup();

    protected:
        void onUpdate() override;
    };
}

