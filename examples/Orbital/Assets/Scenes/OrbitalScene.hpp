#pragma once

// HG::Core
#include <HG/Core/Scene.hpp> // Required for inheritance

class OrbitalScene : public HG::Core::Scene
{
public:
    void start() override;
};
