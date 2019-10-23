#include <HG/Core/Application.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Physics/Base/PhysicsController.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/SystemController.hpp>
#include <gtest/gtest.h>
#include <stdint-gcc.h>

enum class Actions
{
    FirstSceneAllocated,
    FirstSceneDeallocated,
    FirstSceneStarted,
    SecondSceneAllocated,
    SecondSceneDeallocated,
    SecondSceneStarted,

    PhysicsControllerAllocated,
    PhysicsControllerDeallocated,
    PhysicsControllerTicked,

    SystemControllerAllocated,
    SystemControllerDeallocated,
    SystemControllerInited,
    SystemControllerDeinited,
    SystemControllerWindowCreated,
    SystemControllerWindowClosed,
    SystemControllerBuffersSwapped,
    SystemControllerEventsPolled,
};

static std::vector<Actions> TestActions;

class FirstScene : public HG::Core::Scene
{
public:
    FirstScene()
    {
        TestActions.push_back(Actions::FirstSceneAllocated);
    }

    ~FirstScene() override
    {
        TestActions.push_back(Actions::FirstSceneDeallocated);
    }

    void start() override
    {
        TestActions.push_back(Actions::FirstSceneStarted);
    }
};

class SecondScene : public HG::Core::Scene
{
public:
    SecondScene()
    {
        TestActions.push_back(Actions::SecondSceneAllocated);
    }

    ~SecondScene() override
    {
        TestActions.push_back(Actions::SecondSceneDeallocated);
    }

    void start() override
    {
        TestActions.push_back(Actions::SecondSceneStarted);
    }
};

static std::string titleName = "TestWindowName";

TEST(Core, ApplicationInitial)
{
    HG::Core::Application application(titleName);

    ASSERT_NE(application.renderer(), nullptr);
    ASSERT_NE(application.resourceManager(), nullptr);
    ASSERT_NE(application.timeStatistics(), nullptr);
    ASSERT_NE(application.countStatistics(), nullptr);
    ASSERT_NE(application.threadPool(), nullptr);
    ASSERT_NE(application.resourceCache(), nullptr);
    ASSERT_NE(application.input(), nullptr);

    ASSERT_EQ(application.scene(), nullptr);
    ASSERT_EQ(application.title(), titleName);
    ASSERT_EQ(application.systemController(), nullptr);
    ASSERT_EQ(application.physicsController(), nullptr);
}

TEST(Core, ApplicationSceneSwitch)
{
    TestActions.clear();
    {
        HG::Core::Application application(titleName);

        auto* firstScene = new FirstScene;

        ASSERT_EQ(application.scene(), nullptr);

        application.setScene(firstScene);

        ASSERT_EQ(application.scene(), firstScene);

        auto* secondScene = new SecondScene;

        application.setScene(secondScene);

        ASSERT_EQ(application.scene(), secondScene);

        application.performCycle();
    }

    std::vector<Actions> expected = {Actions::FirstSceneAllocated,
                                     Actions::SecondSceneAllocated,
                                     Actions::FirstSceneDeallocated,
                                     Actions::SecondSceneStarted,
                                     Actions::SecondSceneDeallocated};

    ASSERT_EQ(TestActions, expected);
}

class TestPhysicsController : public HG::Physics::Base::PhysicsController
{
public:
    explicit TestPhysicsController(HG::Core::Application* application) :
        HG::Physics::Base::PhysicsController(application)
    {
        TestActions.push_back(Actions::PhysicsControllerAllocated);
    }

    ~TestPhysicsController() override
    {
        TestActions.push_back(Actions::PhysicsControllerDeallocated);
    }

    void tick(std::chrono::microseconds deltaTime) override
    {
        TestActions.push_back(Actions::PhysicsControllerTicked);
    }
};

TEST(Core, ApplicationPhysicsController)
{
    TestActions.clear();
    {
        HG::Core::Application application(titleName);

        auto* physicsController = new TestPhysicsController(&application);

        application.setPhysicsController(physicsController);

        ASSERT_EQ(application.physicsController(), physicsController);

        application.performCycle();
    }

    std::vector<Actions> expected = {
        Actions::PhysicsControllerAllocated, Actions::PhysicsControllerTicked, Actions::PhysicsControllerDeallocated};

    ASSERT_EQ(TestActions, expected);
}

class TestPipeline : public HG::Rendering::Base::RenderingPipeline
{
public:
    explicit TestPipeline(HG::Core::Application* application) : HG::Rendering::Base::RenderingPipeline(application)
    {
    }

    void clear(HG::Utils::Color color) override
    {
    }

    void render(const std::vector<HG::Core::GameObject*>& objects) override
    {
    }

    bool render(HG::Rendering::Base::RenderBehaviour* behaviour) override
    {
        return true;
    }

    void blit(HG::Rendering::Base::RenderTarget* target, HG::Rendering::Base::BlitData* blitData) override
    {
    }

    void getTextureRegion(HG::Rendering::Base::Texture* texture, glm::ivec2 tl, glm::ivec2 br, uint8_t* data) override
    {
    }
};

class TestSystemController : public HG::Rendering::Base::SystemController
{
public:
    explicit TestSystemController(HG::Core::Application* application) : SystemController(application)
    {
        TestActions.push_back(Actions::SystemControllerAllocated);
    }

    ~TestSystemController() override
    {
        TestActions.push_back(Actions::SystemControllerDeallocated);
    }

    void changeTitle(std::string title) override
    {
    }

    void closeWindow() override
    {
        TestActions.push_back(Actions::SystemControllerWindowClosed);
    }

    void swapBuffers() override
    {
        TestActions.push_back(Actions::SystemControllerBuffersSwapped);
    }

    [[nodiscard]] HG::Utils::Rect viewport() const override
    {
        return HG::Utils::Rect();
    }

protected:
    bool onInit() override
    {
        TestActions.push_back(Actions::SystemControllerInited);
        return true;
    }

public:
    bool isWindowFocused() override
    {
        return false;
    }

protected:
    void onPollEvents() override
    {
        TestActions.push_back(Actions::SystemControllerEventsPolled);
    }
    bool onCreateWindow(std::uint32_t width, std::uint32_t height, std::string title) override
    {
        TestActions.push_back(Actions::SystemControllerWindowCreated);
        return true;
    }
    void onDeinit() override
    {
        TestActions.push_back(Actions::SystemControllerDeinited);
    }
};

class TestStopScene : public HG::Core::Scene
{
public:
    void start() override
    {
        application()->stop();
    }
};

TEST(Core, ApplicationSystemControllerFlow)
{
    TestActions.clear();
    {
        HG::Core::Application application(titleName);

        auto* pipeline         = new TestPipeline(&application);
        auto* systemController = new TestSystemController(&application);

        application.renderer()->setPipeline(pipeline);

        ASSERT_EQ(application.renderer()->pipeline(), pipeline);

        application.setSystemController(systemController);

        ASSERT_EQ(application.systemController(), systemController);

        auto* scene = new TestStopScene;

        application.setScene(scene);

        ASSERT_EQ(application.init(), true);

        auto result = application.exec();

        ASSERT_EQ(result, 0);
    }

    std::vector<Actions> expected = {
        Actions::SystemControllerAllocated,
        Actions::SystemControllerInited,
        Actions::SystemControllerWindowCreated,
        Actions::SystemControllerEventsPolled,
        //        Actions::SystemControllerBuffersSwapped, // Pipeline implementation calls this
        Actions::SystemControllerDeinited,
        Actions::SystemControllerWindowClosed,
        Actions::SystemControllerDeallocated,
    };

    ASSERT_EQ(TestActions, expected);
}
