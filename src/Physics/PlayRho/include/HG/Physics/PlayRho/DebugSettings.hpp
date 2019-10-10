#pragma once

// PlayRho
#include <PlayRho/PlayRho.hpp>

namespace HG::Physics::PlayRho
{
namespace
{
using namespace playrho;
}

struct DebugSettings
{
    bool enabled = false;

    float maxTranslation = static_cast<float>(playrho::Real{playrho::DefaultMaxTranslation / playrho::Meter});
    float maxRotation    = 90; // in degrees

    float dt    = 1.0f / 60; // in seconds.
    float minDt = 1.0f / 120;
    float maxDt = 1.0f / 5;

    float minStillTimeToSleep =
        static_cast<float>(playrho::Real{playrho::DefaultMinStillTimeToSleep / playrho::Second});
    float maxLinearCorrection =
        static_cast<float>(playrho::Real{playrho::DefaultMaxLinearCorrection / playrho::Meter}); // in meters
    float maxAngularCorrection =
        static_cast<float>(playrho::Real{playrho::DefaultMaxAngularCorrection / playrho::Degree}); // in degrees

    /// @brief Linear slop.
    /// @note Explicily coded to default to the same value as used in Erin's Box2D 2.3.2
    float linearSlop = static_cast<float>(playrho::Real{playrho::DefaultLinearSlop / playrho::Meter});

    /// @brief Angular slop.
    /// @note Explicily coded to default to the same value as used in Erin's Box2D 2.3.2
    float angularSlop = static_cast<float>(playrho::Real{playrho::DefaultAngularSlop / playrho::Degree}); // in degrees

    float regMinSeparation = static_cast<float>(playrho::Real{playrho::DefaultLinearSlop / playrho::Meter}) * -3.0f;
    float toiMinSeparation = static_cast<float>(playrho::Real{playrho::DefaultLinearSlop / playrho::Meter}) * -1.5f;

    float aabbExtension =
        static_cast<float>(playrho::Real{playrho::DefaultAabbExtension / playrho::Meter}); // in meters
    float tolerance =
        static_cast<float>(playrho::Real{playrho::DefaultLinearSlop / playrho::Real{4} / Meter}); // in meters

    int regPosResRate         = 20; // in percent
    int toiPosResRate         = 75; // in percent
    int regVelocityIterations = 8;
    int regPositionIterations = 3;
    int toiVelocityIterations = 8;
    int toiPositionIterations = 20;
    int maxSubSteps           = playrho::DefaultMaxSubSteps;
    int maxToiRootIters       = playrho::DefaultMaxToiRootIters;
    bool drawShapes           = true;
    bool drawSkins            = false;
    bool drawLabels           = false;
    bool drawJoints           = true;
    bool drawAABBs            = true;
    bool drawContactPoints    = false;
    bool drawContactNormals   = false;
    bool drawContactImpulse   = false;
    bool drawFrictionImpulse  = false;
    bool drawCOMs             = false;
    bool enableWarmStarting   = true;
    bool enableContinuous     = true;
    bool enableSubStepping    = false;
    bool enableSleep          = true;
    bool pause                = false;
    bool singleStep           = false;
};
} // namespace HG::Physics::PlayRho
