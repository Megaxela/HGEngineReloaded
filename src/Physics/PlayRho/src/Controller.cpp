#include <chrono>
#include "Controller.hpp"
#include <PlayRho/PlayRho.hpp>
#include <Color.hpp>
#include <DebugDraw.hpp>
#include <Application.hpp>
#include <set>

using FixtureSet = std::set<playrho::d2::Fixture*>;

namespace {

    using namespace playrho;

    struct Settings
    {
        float maxTranslation = static_cast<float>(playrho::Real{
            playrho::DefaultMaxTranslation / playrho::Meter});
        float maxRotation = 90; // in degrees

        float dt = 1.0f / 60; // in seconds.
        float minDt = 1.0f / 120;
        float maxDt = 1.0f / 5;

        float minStillTimeToSleep = static_cast<float>(playrho::Real{
            playrho::DefaultMinStillTimeToSleep / playrho::Second});
        float maxLinearCorrection = static_cast<float>(playrho::Real{
            playrho::DefaultMaxLinearCorrection / playrho::Meter}); // in meters
        float maxAngularCorrection = static_cast<float>(playrho::Real{
            playrho::DefaultMaxAngularCorrection / playrho::Degree}); // in degrees

        /// @brief Linear slop.
        /// @note Explicily coded to default to the same value as used in Erin's Box2D 2.3.2
        float linearSlop = static_cast<float>(playrho::Real{
            playrho::DefaultLinearSlop / playrho::Meter});

        /// @brief Angular slop.
        /// @note Explicily coded to default to the same value as used in Erin's Box2D 2.3.2
        float angularSlop = static_cast<float>(playrho::Real{
            playrho::DefaultAngularSlop / playrho::Degree}); // in degrees

        float regMinSeparation = static_cast<float>(playrho::Real{
            playrho::DefaultLinearSlop / playrho::Meter}) * -3.0f;
        float toiMinSeparation = static_cast<float>(playrho::Real{
            playrho::DefaultLinearSlop / playrho::Meter}) * -1.5f;

        float aabbExtension = static_cast<float>(playrho::Real{playrho::DefaultAabbExtension / playrho::Meter}); // in meters
        float tolerance = static_cast<float>(playrho::Real{playrho::DefaultLinearSlop / playrho::Real{4} / Meter}); // in meters

        float cameraZoom = 1.0f;

        int regPosResRate = 20; // in percent
        int toiPosResRate = 75; // in percent
        int regVelocityIterations = 8;
        int regPositionIterations = 3;
        int toiVelocityIterations = 8;
        int toiPositionIterations = 20;
        int maxSubSteps = playrho::DefaultMaxSubSteps;
        int maxToiRootIters = playrho::DefaultMaxToiRootIters;
        bool drawShapes = true;
        bool drawSkins = false;
        bool drawLabels = false;
        bool drawJoints = true;
        bool drawAABBs = false;
        bool drawContactPoints = false;
        bool drawContactNormals = false;
        bool drawContactImpulse = false;
        bool drawFrictionImpulse = false;
        bool drawCOMs = false;
        bool enableWarmStarting = true;
        bool enableContinuous = true;
        bool enableSubStepping = false;
        bool enableSleep = true;
        bool pause = false;
        bool singleStep = false;
    };

    struct VisitorData
    {
        PLAYRHO_PHYSICS_MODULE_NS::DebugDraw* drawer;
        playrho::d2::Transformation xf;
        UTILS_MODULE_NS::Color color;
        bool skins;
    };

    static void DrawCorner(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                           playrho::Length2 p,
                           playrho::Length r,
                           playrho::Angle a0,
                           playrho::Angle a1,
                           UTILS_MODULE_NS::Color color)
    {
        const auto angleDiff = playrho::GetRevRotationalAngle(a0, a1);
        auto lastAngle = 0_deg;
        for (auto angle = 5_deg; angle < angleDiff; angle += 5_deg)
        {
            const auto c0 = p + r * playrho::d2::UnitVec::Get(a0 + lastAngle);
            const auto c1 = p + r * playrho::d2::UnitVec::Get(a0 + angle);
            drawer.drawSegment(c0, c1, color);
            lastAngle = angle;
        }
        {
            const auto c0 = p + r * playrho::d2::UnitVec::Get(a0 + lastAngle);
            const auto c1 = p + r * playrho::d2::UnitVec::Get(a1);
            drawer.drawSegment(c0, c1, color);
        }
    }

    static void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                     const playrho::d2::DistanceProxy& shape,
                     UTILS_MODULE_NS::Color color,
                     bool skins,
                     playrho::d2::Transformation xf)
    {
        const auto vertexCount = shape.GetVertexCount();
        auto vertices = std::vector<playrho::Length2>(vertexCount);
        for (auto i = decltype(vertexCount){0}; i < vertexCount; ++i)
        {
            vertices[i] = Transform(shape.GetVertex(i), xf);
        }
        const auto fillColor = UTILS_MODULE_NS::Color{
            0.5f * color.redF(),
            0.5f * color.greenF(),
            0.5f * color.blueF(),
            0.5f
        };
        drawer.drawSolidPolygon(&vertices[0], vertexCount, fillColor);
        drawer.drawPolygon(&vertices[0], vertexCount, color);

        if (!skins)
        {
            return;
        }

        const auto skinColor = UTILS_MODULE_NS::Color{
            color.redF() * 0.6f,
            color.greenF() * 0.6f,
            color.blueF() * 0.6f
        };
        const auto r = GetVertexRadius(shape);
        for (auto i = decltype(vertexCount){0}; i < vertexCount; ++i)
        {
            if (i > 0)
            {
                const auto worldNormal0 = Rotate(shape.GetNormal(i - 1), xf.q);
                const auto p0 = vertices[i-1] + worldNormal0 * r;
                const auto p1 = vertices[i] + worldNormal0 * r;
                drawer.drawSegment(p0, p1, skinColor);
                const auto normal1 = shape.GetNormal(i);
                const auto worldNormal1 = Rotate(normal1, xf.q);
                const auto angle0 = GetAngle(worldNormal0);
                const auto angle1 = GetAngle(worldNormal1);
                DrawCorner(drawer, vertices[i], r, angle0, angle1, skinColor);
            }
        }
        if (vertexCount > 1)
        {
            const auto worldNormal0 = Rotate(shape.GetNormal(vertexCount - 1), xf.q);
            drawer.drawSegment(
                vertices[vertexCount - 1] + worldNormal0 * r,
                vertices[0] + worldNormal0 * r,
                skinColor
            );
            const auto worldNormal1 = Rotate(shape.GetNormal(0), xf.q);
            const auto angle0 = GetAngle(worldNormal0);
            const auto angle1 = GetAngle(worldNormal1);
            DrawCorner(drawer, vertices[0], r, angle0, angle1, skinColor);
        }
        else if (vertexCount == 1)
        {
            DrawCorner(drawer, vertices[0], r, 0_deg, 360_deg, skinColor);
        }
    }

    void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
              const playrho::d2::DiskShapeConf& shape,
              UTILS_MODULE_NS::Color color,
              playrho::d2::Transformation xf)
    {
        const auto center = Transform(shape.GetLocation(), xf);
        const auto radius = shape.GetRadius();
        const auto fillColor = UTILS_MODULE_NS::Color{
            0.5f * color.redF(),
            0.5f * color.greenF(),
            0.5f * color.blueF(),
            0.5f
        };
        drawer.drawSolidCircle(center, radius, fillColor);
        drawer.drawCircle(center, radius, color);

        // Draw a line fixed in the circle to animate rotation.
        const auto axis = Rotate(playrho::Vec2{1, 0}, xf.q);
        drawer.drawSegment(center, center + radius * axis, color);
    }

    void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
              const playrho::d2::EdgeShapeConf& shape,
              UTILS_MODULE_NS::Color color,
              bool skins,
              playrho::d2::Transformation xf)
    {
        const auto v1 = Transform(shape.GetVertexA(), xf);
        const auto v2 = Transform(shape.GetVertexB(), xf);
        drawer.drawSegment(v1, v2, color);

        if (skins)
        {
            const auto r = GetVertexRadius(shape);
            if (r > 0_m)
            {
                const auto skinColor = UTILS_MODULE_NS::Color{
                    color.redF() * 0.6f,
                    color.greenF() * 0.6f,
                    color.blueF() * 0.6f
                };
                const auto worldNormal0 = GetFwdPerpendicular(playrho::d2::GetUnitVector(v2 - v1));
                const auto offset = worldNormal0 * r;
                drawer.drawSegment(v1 + offset, v2 + offset, skinColor);
                drawer.drawSegment(v1 - offset, v2 - offset, skinColor);

                const auto angle0 = GetAngle(worldNormal0);
                const auto angle1 = GetAngle(-worldNormal0);
                DrawCorner(drawer, v2, r, angle0, angle1, skinColor);
                DrawCorner(drawer, v1, r, angle1, angle0, skinColor);
            }
        }
    }

    void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
              const playrho::d2::ChainShapeConf& shape,
              UTILS_MODULE_NS::Color color,
              bool skins,
              playrho::d2::Transformation xf)
    {
        const auto count = shape.GetVertexCount();
        const auto r = GetVertexRadius(shape);
        const auto skinColor = UTILS_MODULE_NS::Color{
            color.redF() * 0.6f,
            color.greenF() * 0.6f,
            color.blueF() * 0.6f
        };

        auto v1 = Transform(shape.GetVertex(0), xf);
        for (auto i = decltype(count){1}; i < count; ++i)
        {
            const auto v2 = Transform(shape.GetVertex(i), xf);
            drawer.drawSegment(v1, v2, color);
            if (skins && r > 0_m)
            {
                const auto worldNormal0 = GetFwdPerpendicular(playrho::d2::GetUnitVector(v2 - v1));
                const auto offset = worldNormal0 * r;
                drawer.drawSegment(v1 + offset, v2 + offset, skinColor);
                drawer.drawSegment(v1 - offset, v2 - offset, skinColor);
                const auto angle0 = GetAngle(worldNormal0);
                const auto angle1 = GetAngle(-worldNormal0);
                DrawCorner(drawer, v2, r, angle0, angle1, skinColor);
                DrawCorner(drawer, v1, r, angle1, angle0, skinColor);
            }
            v1 = v2;
        }
    }

    void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
              const playrho::d2::PolygonShapeConf& shape,
              UTILS_MODULE_NS::Color color,
              bool skins,
              playrho::d2::Transformation xf)
    {
        Draw(drawer, GetChild(shape, 0), color, skins, xf);
    }

    void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
              const playrho::d2::MultiShapeConf& shape,
              UTILS_MODULE_NS::Color color,
              bool skins,
              playrho::d2::Transformation xf)
    {
        const auto count = GetChildCount(shape);
        for (auto i = decltype(count){0}; i < count; ++i)
        {
            Draw(drawer, GetChild(shape, i), color, skins, xf);
        }
    }

    static void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                     const playrho::d2::Fixture& fixture,
                     const UTILS_MODULE_NS::Color& color,
                     bool skins)
    {
        const auto xf = GetTransformation(fixture);
#if 0
        auto visitor = FunctionalVisitor{};
    visitor.visitDisk = [&](const d2::DiskShapeConf& shape) {
        Draw(drawer, shape, color, xf);
    };
    visitor.visitEdge = [&](const d2::EdgeShapeConf& shape) {
        Draw(drawer, shape, color, skins, xf);
    };
    visitor.visitPolygon = [&](const d2::PolygonShapeConf& shape) {
        Draw(drawer, shape, color, skins, xf);
    };
    visitor.visitChain = [&](const d2::ChainShapeConf& shape) {
        Draw(drawer, shape, color, skins, xf);
    };
    visitor.visitMulti = [&](const d2::MultiShapeConf& shape) {
        Draw(drawer, shape, color, skins, xf);
    };
#else
        auto visitor = VisitorData{};
        visitor.drawer = &drawer;
        visitor.xf = xf;
        visitor.color = color;
        visitor.skins = skins;
#endif
        playrho::Visit(fixture.GetShape(), &visitor);
    }

    static UTILS_MODULE_NS::Color GetColor(const playrho::d2::Body& body)
    {
        if (!body.IsEnabled())
        {
            return UTILS_MODULE_NS::Color{0.5f, 0.5f, 0.3f};
        }
        if (body.GetType() == playrho::BodyType::Static)
        {
            return UTILS_MODULE_NS::Color{0.5f, 0.9f, 0.5f};
        }
        if (body.GetType() == playrho::BodyType::Kinematic)
        {
            return UTILS_MODULE_NS::Color{0.5f, 0.5f, 0.9f};
        }
        if (!body.IsAwake())
        {
            return UTILS_MODULE_NS::Color{0.75f, 0.75f, 0.75f};
        }
        return UTILS_MODULE_NS::Color{0.9f, 0.7f, 0.7f};
    }

    static bool Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                     const playrho::d2::Body& body,
                     bool skins,
                     const FixtureSet& selected)
    {
        auto found = false;
        const auto bodyColor = GetColor(body);
        const auto selectedColor = UTILS_MODULE_NS::Color::brighten(bodyColor, 1.3f);
        for (auto&& fixture: body.GetFixtures())
        {
            const auto& f = playrho::GetRef(fixture);
            auto color = bodyColor;

            if (selected.find(const_cast<playrho::d2::Fixture*>(&f)) != end(selected))
            {
                color = selectedColor;
                found = true;
            }
            Draw(drawer, f, color, skins);
        }
        return found;
    }

    static void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                     const playrho::d2::Joint& joint)
    {
        const auto p1 = joint.GetAnchorA();
        const auto p2 = joint.GetAnchorB();

        const UTILS_MODULE_NS::Color color{0.5f, 0.8f, 0.8f};

        switch (GetType(joint))
        {
        case playrho::d2::JointType::Distance:
            drawer.drawSegment(p1, p2, color);
            break;

        case playrho::d2::JointType::Pulley:
        {
            const auto pulley = static_cast<const playrho::d2::PulleyJoint&>(joint);
            const auto s1 = pulley.GetGroundAnchorA();
            const auto s2 = pulley.GetGroundAnchorB();
            drawer.drawSegment(s1, p1, color);
            drawer.drawSegment(s2, p2, color);
            drawer.drawSegment(s1, s2, color);
        }
            break;

        case playrho::d2::JointType::Target:
            // don't draw this
            break;

        default:
        {
            const auto bodyA = joint.GetBodyA();
            const auto bodyB = joint.GetBodyB();
            const auto x1 = bodyA->GetTransformation().p;
            const auto x2 = bodyB->GetTransformation().p;
            drawer.drawSegment(x1, p1, color);
            drawer.drawSegment(p1, p2, color);
            drawer.drawSegment(x2, p2, color);
        }
        }
    }

    static void Draw(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                     const playrho::d2::AABB& aabb,
                     const UTILS_MODULE_NS::Color& color)
    {
        playrho::Length2 vs[4];
        vs[0] = playrho::Length2{aabb.ranges[0].GetMin(), aabb.ranges[1].GetMin()};
        vs[1] = playrho::Length2{aabb.ranges[0].GetMax(), aabb.ranges[1].GetMin()};
        vs[2] = playrho::Length2{aabb.ranges[0].GetMax(), aabb.ranges[1].GetMax()};
        vs[3] = playrho::Length2{aabb.ranges[0].GetMin(), aabb.ranges[1].GetMax()};
        drawer.drawPolygon(vs, 4, color);
    }

    bool DrawWorld(PLAYRHO_PHYSICS_MODULE_NS::DebugDraw& drawer,
                   const playrho::d2::World& world,
                   const Settings& settings,
                   const FixtureSet& selected)
    {
        auto found = false;

        if (settings.drawShapes)
        {
            const auto drawLabels = settings.drawLabels;
            const auto drawSkins = settings.drawSkins;

            for (auto&& body: world.GetBodies())
            {
                const auto b = playrho::GetPtr(body);
                if (Draw(drawer, *b, drawSkins, selected))
                {
                    found = true;
                }
                if (drawLabels)
                {
                    // Use center of mass instead of body center since body center may not
//                    drawer.drawString(b->GetWorldCenter(), Drawer::Center, "%d", GetWorldIndex(b));
                }
            }
        }

        if (settings.drawJoints)
        {
            for (auto&& j: world.GetJoints())
            {
                Draw(drawer, *j);
            }
        }

        if (settings.drawAABBs)
        {
            const auto color = UTILS_MODULE_NS::Color{0.9f, 0.3f, 0.9f};
            const auto root = world.GetTree().GetRootIndex();
            if (root != playrho::d2::DynamicTree::GetInvalidSize())
            {
                const auto worldAabb = world.GetTree().GetAABB(root);
                Draw(drawer, worldAabb, color);
                Query(world.GetTree(), worldAabb, [&](playrho::d2::DynamicTree::Size id) {
                    Draw(drawer, world.GetTree().GetAABB(id), color);
                    return playrho::d2::DynamicTreeOpcode::Continue;
                });
            }
        }

        if (settings.drawCOMs)
        {
            const auto k_axisScale = 0.4_m;
            const auto red = UTILS_MODULE_NS::Color{1.0f, 0.0f, 0.0f};
            const auto green = UTILS_MODULE_NS::Color{0.0f, 1.0f, 0.0f};
            for (auto&& body: world.GetBodies())
            {
                const auto b = playrho::GetPtr(body);
                const auto massScale = std::pow(static_cast<float>(playrho::StripUnit(GetMass(*b))), 1.0f/3);
                auto xf = b->GetTransformation();
                xf.p = b->GetWorldCenter();
                const auto p1 = xf.p;
                drawer.drawSegment(p1, p1 + massScale * k_axisScale * GetXAxis(xf.q), red);
                drawer.drawSegment(p1, p1 + massScale * k_axisScale * GetYAxis(xf.q), green);
            }
        }

        return found;
    }
}

PLAYRHO_PHYSICS_MODULE_NS::Controller::Controller(::CORE_MODULE_NS::Application *parent) :
    PhysicsController(parent),
    m_drawer(parent->renderer()->gizmos()),
    m_stepConfiguration(),
    m_world()
{

}

playrho::d2::World *PLAYRHO_PHYSICS_MODULE_NS::Controller::world()
{
    return &m_world;
}

void PLAYRHO_PHYSICS_MODULE_NS::Controller::tick(std::chrono::microseconds deltaTime)
{
    m_stepConfiguration.SetTime(
        deltaTime.count() / 1000000.0f
    );

    m_world.Step(m_stepConfiguration);

    Settings s{};

    DrawWorld(m_drawer, m_world, s, FixtureSet());

//    playrho::d2::Fixture* fix;
//
//    playrho::Visit(fix->GetShape(), nullptr);

//    playrho::d2::Shape::Accept(fix->GetShape(), )
}

// Compiletime visitor implementation
namespace playrho {

    template <>
    bool Visit(const playrho::d2::DiskShapeConf& shape, void* userData)
    {
        const auto data = static_cast<VisitorData*>(userData);
        Draw(*(data->drawer), shape, data->color, data->xf);
        return true;
    }

    template <>
    bool Visit(const playrho::d2::EdgeShapeConf& shape, void* userData)
    {
        const auto data = static_cast<VisitorData*>(userData);
        Draw(*(data->drawer), shape, data->color, data->skins, data->xf);
        return true;
    }

    template <>
    bool Visit(const playrho::d2::PolygonShapeConf& shape, void* userData)
    {
        const auto data = static_cast<VisitorData*>(userData);
        Draw(*(data->drawer), shape, data->color, data->skins, data->xf);
        return true;
    }

    template <>
    bool Visit(const playrho::d2::ChainShapeConf& shape, void* userData)
    {
        const auto data = static_cast<VisitorData*>(userData);
        Draw(*(data->drawer), shape, data->color, data->skins, data->xf);
        return true;
    }

    template <>
    bool Visit(const playrho::d2::MultiShapeConf& shape, void* userData)
    {
        const auto data = static_cast<VisitorData*>(userData);
        Draw(*(data->drawer), shape, data->color, data->skins, data->xf);
        return true;
    }

} // namespace playrho