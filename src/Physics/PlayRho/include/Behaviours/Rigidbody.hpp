#pragma once

#include <Behaviour.hpp>
#include "PlayRho/PlayRho.hpp"

namespace PLAYRHO_PHYSICS_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes rigidbody, based on
     * PlayRho physics.
     */
    class Rigidbody : public CORE_MODULE_NS::Behaviour
    {
    public:

        /**
         * @brief Default constructor.
         */
        Rigidbody();

        /**
         * @brief Constructor from body configuration.
         * @param bodyConfiguration Body configuration.
         */
        explicit Rigidbody(playrho::d2::BodyConf bodyConfiguration);

        /**
         * @brief Constructor from body.
         * @param body Pointer to body, created with PlayRho controller's world.
         */
        explicit Rigidbody(playrho::d2::Body* body);

        /**
         * @brief Method for getting playrho body object.
         * @return Pointer to body object.
         */
        playrho::d2::Body* body() const;

    protected:
        void onStart() override;

        void onUpdate() override;


    private:
        void applyCurrentGameObjectTransform();

        playrho::d2::BodyConf m_configuration;
        playrho::d2::Body* m_body;
    };
}