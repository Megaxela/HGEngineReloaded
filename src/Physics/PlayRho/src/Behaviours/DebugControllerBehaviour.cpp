// HG::Physics::PlayRho
#include <Behaviours/DebugControllerBehaviour.hpp>
#include <Controller.hpp>

// HG::Core
#include <Scene.hpp>
#include <Application.hpp>

// ImGUI
#include <imgui.h>
#include <imgui_internal.h>


HG::Physics::PlayRho::Behaviours::DebugControllerBehaviour::DebugControllerBehaviour()
{

}

void HG::Physics::PlayRho::Behaviours::DebugControllerBehaviour::onStart()
{
    setEnabled(false);
}

void HG::Physics::PlayRho::Behaviours::DebugControllerBehaviour::onUpdate()
{
    auto controller = scene()->application()->physicsController<Controller>();

    auto settings = controller->debugSettings();
    auto stepConf = controller->stepConfiguration();

    bool opened = true;

    if (ImGui::Begin("PlayRho settings", &opened))
    {
        if (!opened)
        {
            setEnabled(false);
        }

        int tmp;

        ImGui::PushItemWidth(200);

        tmp = stepConf->regVelocityIterations;
        ImGui::SliderInt("Regular Velocity Iterations", &tmp, 0, 100);
        stepConf->regVelocityIterations = static_cast<StepConf::iteration_type>(tmp);
        tooltip("The number of iterations of velocity resolution that will be done in the step.");

        tmp = stepConf->regPositionIterations;
        ImGui::SliderInt("Regular Position Iterations", &tmp, 0, 100);
        stepConf->regPositionIterations= static_cast<StepConf::iteration_type>(tmp);
        tooltip("This is the maximum number of iterations of position resolution that will\n"
                "be done before leaving any remaining unsatisfied positions for the next step.\n"
                "In this context, positions are satisfied when the minimum separation is greater than\n"
                "or equal to the regular minimum separation amount.");

        tmp = stepConf->toiVelocityIterations;
        ImGui::SliderInt("Time Of Impact Velocity Iterations", &tmp, 0, 100);
        stepConf->toiVelocityIterations = static_cast<StepConf::iteration_type>(tmp);
        tooltip("This is the number of iterations of velocity resolution that will be done in the step.");

        tmp = stepConf->toiPositionIterations;
        ImGui::SliderInt("Time Of Impact Position Iterations", &tmp, 0, 100);
        stepConf->toiPositionIterations = static_cast<StepConf::iteration_type>(tmp);
        tooltip("This value is the maximum number of iterations of position resolution that will\n"
                "be done before leaving any remaining unsatisfied positions for the next step.\n"
                "In this context, positions are satisfied when the minimum separation is greater than\n"
                "or equal to the TOI minimum separation amount.");

        tmp = stepConf->maxToiRootIters;
        ImGui::SliderInt("Maximum Time Of Impact Root Finder Iterations", &tmp, 0, 100);
        stepConf->maxToiRootIters = static_cast<StepConf::iteration_type>(tmp);

        tmp = stepConf->maxToiIters;
        ImGui::SliderInt("Time Of Impact Velocity Iterations", &tmp, 0, 100);
        stepConf->maxToiIters = static_cast<StepConf::iteration_type>(tmp);

        tmp = stepConf->maxDistanceIters;
        ImGui::SliderInt("Maximum Distance Iterations", &tmp, 0, 100);
        stepConf->maxDistanceIters= static_cast<StepConf::iteration_type>(tmp);

        tmp = stepConf->maxSubSteps;
        ImGui::SliderInt("Maximum Sub-Steps", &tmp, 0, 100);
        stepConf->maxSubSteps = static_cast<StepConf::iteration_type>(tmp);
        tooltip("This is the maximum number of sub-steps per contact in continuous physics simulation.\n"
                "In other words, this is the maximum number of times in a world step that a contact will\n"
                "have continuous collision resolution done for it.");

        ImGui::Checkbox("Do Warm Start", &stepConf->doWarmStart);
        tooltip("Whether or not to perform warm starting (in the regular phase).");

        ImGui::Checkbox("Do Time Of Impact", &stepConf->doToi);
        tooltip("Whether or not to perform any time of impact (TOI) calculations used for doing\n"
                "continuous collision detection. Without this, steps can potentially be computed\n"
                "faster but with increased chance of bodies passing unobstructed through other bodies\n"
                "(a process called \"tunneling\") even when they're not supposed to be able to go through\n"
                "them.");

        ImGui::Checkbox("Do Blocksolve algo", &stepConf->doBlocksolve);

        ImGui::InputFloat ("Min Still Time To Sleep", &stepConf->minStillTimeToSleep, 0.0f, 0.0f, "%.3f s");
        tooltip("The time that a body must be still before it will be put to sleep.");

        ImGui::InputFloat("Linear Slop", (float*) &stepConf->linearSlop, 0.0f, 0.0f, "%.3f meters");
        tooltip("Linear slop for position resolution. Must be greater than 0.");

        ImGui::InputFloat("Angular Slop", (float*) &stepConf->angularSlop, 0.0f, 0.0f, "%.3f radians");
        tooltip("Angular slop. Must be greater than 0.");

        ImGui::InputFloat("Regular Resolution Rate", &stepConf->regResolutionRate);
        tooltip("This scale factor controls how fast positional overlap is resolved.\n"
                "Ideally this would be 1 so that overlap is removed in one time step.\n"
                "However using values close to 1 often lead to overshoot.");

        ImGui::InputFloat("Regular Minimum Separation", &stepConf->regMinSeparation);
        tooltip("This is the minimum amount of separation there must be between regular-phase interacting\n"
                "bodies for intra-step position resolution to be considered successful and end before all\n"
                "of the regular position iterations have been done.");

        ImGui::InputFloat("Regular-phase Minimum Momentum", &stepConf->regMinMomentum, 0.0f, 0.0f, "%.3f Ns");

        ImGui::InputFloat("Time Of Impact Resolution Rate", &stepConf->toiResolutionRate);
        tooltip("This scale factor controls how fast positional overlap is resolved.\n"
                "Ideally this would be 1 so that overlap is removed in one time step.\n"
                "However using values close to 1 often lead to overshoot.");

        ImGui::InputFloat("Time Of Impact Minimum Separation", &stepConf->toiMinSeparation, 0.0f, 0.0f, "%.3f meters");
        tooltip("This is the minimum amount of separation there must be between TOI-phase interacting\n"
                "bodies for intra-step position resolution to be considered successful and end before all\n"
                "of the TOI position iterations have been done.");

        ImGui::InputFloat("Time Of Impact Minimum Momentum", &stepConf->toiMinMomentum, 0.0f, 0.0f, "%.3f meters");

        ImGui::InputFloat("Target Depth", &stepConf->targetDepth, 0.0f, 0.0f, "%.3f meters");
        tooltip("Target depth of overlap for calculating the TOI for CCD eligible bodies.\n"
                "Recommend value that's less than twice the world's minimum vertex radius.");

        // todo: Add tolerance
        ImGui::InputFloat("Velocity Threshold", &stepConf->velocityThreshold, 0.0f, 0.0f, "%.3f m/s");
        tooltip("A velocity threshold for elastic collisions. Any collision with a relative linear\n"
                "velocity below this threshold will be treated as inelastic.");

        ImGui::InputFloat("Maximum Translation", &stepConf->maxTranslation, 0.0f, 0.0f, "%.3f meters");
        tooltip("The maximum amount a body can translate in a single step. This represents\n"
                "an upper bound on the maximum linear velocity of a body of max-translation per time.");

        ImGui::InputFloat("Maximum Rotation", &stepConf->maxRotation, 0.0f, 0.0f, "%.3f radians");
        tooltip("The maximum amount a body can rotate in a single step. This represents\n"
                "an upper bound on the maximum angular speed of a body of max rotation / time.");

        ImGui::InputFloat("Max Linear Correction", &stepConf->maxLinearCorrection, 0.0f, 0.0f, "%.3f meters");
        tooltip("Must be greater than 0 for any positional resolution to get done.\n"
                "This value should be greater than the linear slop value.");

        ImGui::InputFloat("Max Angular Correction", &stepConf->maxAngularCorrection, 0.0f, 0.0f, "%.3f radians");

        ImGui::InputFloat("Linear Sleep Tolerance", &stepConf->linearSleepTolerance, 0.0f, 0.0f, "%.3f m/s");

        ImGui::InputFloat("Angular Sleep Tolerance", &stepConf->angularSleepTolerance, 0.0f, 0.0f, "%.3f radians/s");

        ImGui::InputFloat("AABB Displacement Multiplier", &stepConf->displaceMultiplier);

        ImGui::InputFloat("AABB Extension", &stepConf->aabbExtension, 0.0f, 0.0f, "%.3f meters");

        ImGui::InputFloat("Maximum Circles Ratio", &stepConf->maxCirclesRatio);

        ImGui::Separator();

        ImGui::Checkbox("Enabled Debug View", &settings->enabled);

        if (!settings->enabled)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        // meters
        ImGui::InputFloat("Tolerance",   &settings->tolerance, 0.0f, 0.0f, "%.4f meters");
        ImGui::Checkbox  ("Draw shapes", &settings->drawShapes);
        ImGui::Checkbox  ("Draw skins",  &settings->drawSkins );
        ImGui::Checkbox  ("Draw joints", &settings->drawJoints);
        ImGui::Checkbox  ("Draw AABBs",  &settings->drawAABBs );
        ImGui::Checkbox  ("Draw COMs",   &settings->drawCOMs  );

        if (!settings->enabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar(1);
        }

        ImGui::PopItemWidth();

        ImGui::End();
    }
}

void HG::Physics::PlayRho::Behaviours::DebugControllerBehaviour::tooltip(const char* text)
{
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();

        ImGui::Text(text);

        ImGui::EndTooltip();
    }
}
