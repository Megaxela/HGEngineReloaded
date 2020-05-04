// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Input.hpp>
#include <HG/Core/TimeStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>

// HG::System::PC
#include <HG/System/PC/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// GLFW
#include <GLFW/glfw3.h>

// Incognito
namespace
{
HG::System::PC::SystemController* controller = nullptr;
}

namespace HG::System::PC
{
SystemController::SystemController(HG::Core::Application* application) :
    HG::Rendering::Base::SystemController(application),
    m_window(nullptr)
{
}

SystemController::~SystemController()
{
    glfwTerminate();
    m_window = nullptr;
}

static const char* ImGuiGetClipboardText(void* user_data)
{
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGuiSetClipboardText(void* user_data, const char* text)
{
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}

bool SystemController::onInit()
{
    HGInfo("Initializing GLFW");

    // Setting error callback
    glfwSetErrorCallback([](int code, const char* description) {
        HGError("GLFW Received error #{}, {}", code, description);
    });

    // Initializing GLFW
    if (!glfwInit())
    {
        return false;
    }

    HGInfo("Initialized");

    // Setting OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    return true;
}

void SystemController::onDeinit()
{
}

bool SystemController::onCreateWindow(std::uint32_t width, std::uint32_t height, std::string title)
{
    HGInfo("Creating window {}x{} with title \"{}\"", width, height, title);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    controller = this;

    // Setting event callbacks
    glfwSetKeyCallback(m_window, &SystemController::keyPressCallback);
    glfwSetCursorPosCallback(m_window, &SystemController::cursorPosCallback);
    glfwSetMouseButtonCallback(m_window, &SystemController::mouseButtonCallback);
    glfwSetJoystickCallback(&SystemController::joystickCallback);
    glfwSetFramebufferSizeCallback(m_window, &SystemController::framebufferSizeCallback);
    glfwSetCharCallback(m_window, &SystemController::charCallback);
    glfwSetScrollCallback(m_window, &SystemController::mouseWheelCallback);

    const_cast<HG::Core::Input::Mouse*>(application()->input()->mouse())->setCursorDisabledAction([=](bool disable) {
        if (disable)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            // todo: Add checking is it hidden and hide cursor
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    });

    const_cast<HG::Core::Input::Mouse*>(application()->input()->mouse())->setCursorHiddenAction([=](bool hidden) {
        if (hidden)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    });

    const_cast<HG::Core::Input::Mouse*>(application()->input()->mouse())->setIsCursorDisabledAction([=]() {
        return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    });

    const_cast<HG::Core::Input::Mouse*>(application()->input()->mouse())->setIsCursorHiddenAction([=]() {
        return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
    });

    glfwMakeContextCurrent(m_window);

    glfwSwapInterval(0);

    // Setting default render target size
    controller->application()->renderer()->defaultRenderTarget()->setSize({width, height});

    return true;
}

bool SystemController::isWindowFocused()
{
    return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
}

void SystemController::closeWindow()
{
    glfwDestroyWindow(m_window);
}

void SystemController::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void SystemController::onPollEvents()
{
    // Ticking pushed/released values in input subsystem
    const_cast<HG::Core::Input*>(application()->input())->tickControllers();

    {
        BENCH("GLFW poll events");
        glfwPollEvents();
    }

    {
        BENCH("Handling gamepad events");
        handleGamepadsEvents();
    }

    {
        BENCH("Handling window events");
        handleWindowEvents();
    }
}

void SystemController::handleGamepadsEvents()
{
    for (auto index = GLFW_JOYSTICK_1; index <= GLFW_JOYSTICK_LAST; ++index)
    {
        int present = glfwJoystickPresent(index);

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<std::uint8_t>(index), static_cast<bool>(present));

        if (!present)
        {
            continue;
        }

        int count;

        // Axes
        const float* axes = glfwGetJoystickAxes(index, &count);

        for (int i = 0; i < count; ++i)
        {
            const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
                ->setGamepadAxisValue(static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(i), axes[i]);
        }

        // Buttons
        const auto buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        for (int i = 0; i < count; ++i)
        {
            if (controller->application()->input()->gamepads()->isButtonPressed(static_cast<std::uint8_t>(index),
                                                                                static_cast<std::uint8_t>(i)))
            {
                if (buttons[i] == GLFW_RELEASE)
                {
                    const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
                        ->setGamepadButtonValue(static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(i), false);
                }
            }
            else
            {
                if (buttons[i] == GLFW_PRESS)
                {
                    const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
                        ->setGamepadButtonValue(static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(i), true);
                }
            }
        }
    }
}

void SystemController::joystickCallback(int gamepad, int event)
{
    if (event == GLFW_CONNECTED)
    {
        HGInfo("Gamepad #{} connected.", gamepad);

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<std::uint8_t>(gamepad), true);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        HGInfo("Gamepad #{} disconnected.", gamepad);

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<std::uint8_t>(gamepad), false);
    }
}

void SystemController::cursorPosCallback(GLFWwindow*, double x, double y)
{
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setMousePosition(static_cast<int>(x), static_cast<int>(y));
}

void SystemController::keyPressCallback(GLFWwindow*, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    static std::unordered_map<int, HG::Core::Input::Keyboard::Key> keys = {
        {GLFW_KEY_0, HG::Core::Input::Keyboard::Key::N0},
        {GLFW_KEY_1, HG::Core::Input::Keyboard::Key::N1},
        {GLFW_KEY_2, HG::Core::Input::Keyboard::Key::N2},
        {GLFW_KEY_3, HG::Core::Input::Keyboard::Key::N3},
        {GLFW_KEY_4, HG::Core::Input::Keyboard::Key::N4},
        {GLFW_KEY_5, HG::Core::Input::Keyboard::Key::N5},
        {GLFW_KEY_6, HG::Core::Input::Keyboard::Key::N6},
        {GLFW_KEY_7, HG::Core::Input::Keyboard::Key::N7},
        {GLFW_KEY_8, HG::Core::Input::Keyboard::Key::N8},
        {GLFW_KEY_9, HG::Core::Input::Keyboard::Key::N9},
        {GLFW_KEY_SPACE, HG::Core::Input::Keyboard::Key::Space},
        {GLFW_KEY_APOSTROPHE, HG::Core::Input::Keyboard::Key::SingleQuotation},
        {GLFW_KEY_COMMA, HG::Core::Input::Keyboard::Key::Comma},
        {GLFW_KEY_MINUS, HG::Core::Input::Keyboard::Key::Minus},
        {GLFW_KEY_PERIOD, HG::Core::Input::Keyboard::Key::Dot},
        {GLFW_KEY_SLASH, HG::Core::Input::Keyboard::Key::Slash},
        {GLFW_KEY_SEMICOLON, HG::Core::Input::Keyboard::Key::Semicolon},
        {GLFW_KEY_EQUAL, HG::Core::Input::Keyboard::Key::Equal},
        {GLFW_KEY_A, HG::Core::Input::Keyboard::Key::A},
        {GLFW_KEY_B, HG::Core::Input::Keyboard::Key::B},
        {GLFW_KEY_C, HG::Core::Input::Keyboard::Key::C},
        {GLFW_KEY_D, HG::Core::Input::Keyboard::Key::D},
        {GLFW_KEY_E, HG::Core::Input::Keyboard::Key::E},
        {GLFW_KEY_F, HG::Core::Input::Keyboard::Key::F},
        {GLFW_KEY_G, HG::Core::Input::Keyboard::Key::G},
        {GLFW_KEY_H, HG::Core::Input::Keyboard::Key::H},
        {GLFW_KEY_I, HG::Core::Input::Keyboard::Key::I},
        {GLFW_KEY_J, HG::Core::Input::Keyboard::Key::J},
        {GLFW_KEY_K, HG::Core::Input::Keyboard::Key::K},
        {GLFW_KEY_L, HG::Core::Input::Keyboard::Key::L},
        {GLFW_KEY_M, HG::Core::Input::Keyboard::Key::M},
        {GLFW_KEY_N, HG::Core::Input::Keyboard::Key::N},
        {GLFW_KEY_O, HG::Core::Input::Keyboard::Key::O},
        {GLFW_KEY_P, HG::Core::Input::Keyboard::Key::P},
        {GLFW_KEY_Q, HG::Core::Input::Keyboard::Key::Q},
        {GLFW_KEY_R, HG::Core::Input::Keyboard::Key::R},
        {GLFW_KEY_S, HG::Core::Input::Keyboard::Key::S},
        {GLFW_KEY_T, HG::Core::Input::Keyboard::Key::T},
        {GLFW_KEY_U, HG::Core::Input::Keyboard::Key::U},
        {GLFW_KEY_V, HG::Core::Input::Keyboard::Key::V},
        {GLFW_KEY_W, HG::Core::Input::Keyboard::Key::W},
        {GLFW_KEY_X, HG::Core::Input::Keyboard::Key::X},
        {GLFW_KEY_Y, HG::Core::Input::Keyboard::Key::Y},
        {GLFW_KEY_Z, HG::Core::Input::Keyboard::Key::Z},
        {GLFW_KEY_LEFT_BRACKET, HG::Core::Input::Keyboard::Key::SquareBracketOpen},
        {GLFW_KEY_BACKSLASH, HG::Core::Input::Keyboard::Key::Backslash},
        {GLFW_KEY_RIGHT_BRACKET, HG::Core::Input::Keyboard::Key::SquareBracketClose},
        {GLFW_KEY_GRAVE_ACCENT, HG::Core::Input::Keyboard::Key::Tilda}, // todo: Add world keys here
        {GLFW_KEY_ESCAPE, HG::Core::Input::Keyboard::Key::ESC},
        {GLFW_KEY_ENTER, HG::Core::Input::Keyboard::Key::Return},
        {GLFW_KEY_TAB, HG::Core::Input::Keyboard::Key::Tab},
        {GLFW_KEY_BACKSPACE, HG::Core::Input::Keyboard::Key::Backspace},
        {GLFW_KEY_INSERT, HG::Core::Input::Keyboard::Key::Insert},
        {GLFW_KEY_DELETE, HG::Core::Input::Keyboard::Key::Delete},
        {GLFW_KEY_RIGHT, HG::Core::Input::Keyboard::Key::ArrowRight},
        {GLFW_KEY_LEFT, HG::Core::Input::Keyboard::Key::ArrowLeft},
        {GLFW_KEY_DOWN, HG::Core::Input::Keyboard::Key::ArrowDown},
        {GLFW_KEY_UP, HG::Core::Input::Keyboard::Key::ArrowUp},
        {GLFW_KEY_PAGE_UP, HG::Core::Input::Keyboard::Key::PageUp},
        {GLFW_KEY_PAGE_DOWN, HG::Core::Input::Keyboard::Key::PageDown},
        {GLFW_KEY_HOME, HG::Core::Input::Keyboard::Key::Home},
        {GLFW_KEY_END, HG::Core::Input::Keyboard::Key::End},
        {GLFW_KEY_CAPS_LOCK, HG::Core::Input::Keyboard::Key::CapsLock},
        {GLFW_KEY_NUM_LOCK, HG::Core::Input::Keyboard::Key::NumLock},
        {GLFW_KEY_PRINT_SCREEN, HG::Core::Input::Keyboard::Key::PrintScreen},
        {GLFW_KEY_PAUSE, HG::Core::Input::Keyboard::Key::Pause},
        {GLFW_KEY_F1, HG::Core::Input::Keyboard::Key::F1},
        {GLFW_KEY_F2, HG::Core::Input::Keyboard::Key::F2},
        {GLFW_KEY_F3, HG::Core::Input::Keyboard::Key::F3},
        {GLFW_KEY_F4, HG::Core::Input::Keyboard::Key::F4},
        {GLFW_KEY_F5, HG::Core::Input::Keyboard::Key::F5},
        {GLFW_KEY_F6, HG::Core::Input::Keyboard::Key::F6},
        {GLFW_KEY_F7, HG::Core::Input::Keyboard::Key::F7},
        {GLFW_KEY_F8, HG::Core::Input::Keyboard::Key::F8},
        {GLFW_KEY_F9, HG::Core::Input::Keyboard::Key::F9},
        {GLFW_KEY_F10, HG::Core::Input::Keyboard::Key::F10},
        {GLFW_KEY_F11, HG::Core::Input::Keyboard::Key::F11},
        {GLFW_KEY_F12, HG::Core::Input::Keyboard::Key::F12},
        {GLFW_KEY_F13, HG::Core::Input::Keyboard::Key::F13},
        {GLFW_KEY_F14, HG::Core::Input::Keyboard::Key::F14},
        {GLFW_KEY_F15, HG::Core::Input::Keyboard::Key::F15},
        {GLFW_KEY_F16, HG::Core::Input::Keyboard::Key::F16},
        {GLFW_KEY_F17, HG::Core::Input::Keyboard::Key::F17},
        {GLFW_KEY_F18, HG::Core::Input::Keyboard::Key::F18},
        {GLFW_KEY_F19, HG::Core::Input::Keyboard::Key::F19},
        {GLFW_KEY_F20, HG::Core::Input::Keyboard::Key::F20},
        {GLFW_KEY_F21, HG::Core::Input::Keyboard::Key::F21},
        {GLFW_KEY_F22, HG::Core::Input::Keyboard::Key::F22},
        {GLFW_KEY_F23, HG::Core::Input::Keyboard::Key::F23},
        {GLFW_KEY_F24, HG::Core::Input::Keyboard::Key::F24},
        {GLFW_KEY_F25, HG::Core::Input::Keyboard::Key::F25},
        {GLFW_KEY_KP_0, HG::Core::Input::Keyboard::Key::Num0},
        {GLFW_KEY_KP_1, HG::Core::Input::Keyboard::Key::Num1},
        {GLFW_KEY_KP_2, HG::Core::Input::Keyboard::Key::Num2},
        {GLFW_KEY_KP_3, HG::Core::Input::Keyboard::Key::Num3},
        {GLFW_KEY_KP_4, HG::Core::Input::Keyboard::Key::Num4},
        {GLFW_KEY_KP_5, HG::Core::Input::Keyboard::Key::Num5},
        {GLFW_KEY_KP_6, HG::Core::Input::Keyboard::Key::Num6},
        {GLFW_KEY_KP_7, HG::Core::Input::Keyboard::Key::Num7},
        {GLFW_KEY_KP_8, HG::Core::Input::Keyboard::Key::Num8},
        {GLFW_KEY_KP_9, HG::Core::Input::Keyboard::Key::Num9},
        {GLFW_KEY_KP_DECIMAL, HG::Core::Input::Keyboard::Key::NumDot},
        {GLFW_KEY_KP_DIVIDE, HG::Core::Input::Keyboard::Key::NumDivide},
        {GLFW_KEY_KP_MULTIPLY, HG::Core::Input::Keyboard::Key::NumMultiply},
        {GLFW_KEY_KP_SUBTRACT, HG::Core::Input::Keyboard::Key::NumSubstract},
        {GLFW_KEY_KP_ADD, HG::Core::Input::Keyboard::Key::NumAdd},
        {GLFW_KEY_KP_ENTER, HG::Core::Input::Keyboard::Key::NumReturn},
        {GLFW_KEY_KP_EQUAL, HG::Core::Input::Keyboard::Key::NumEqual},
        {GLFW_KEY_LEFT_SHIFT, HG::Core::Input::Keyboard::Key::LeftShift},
        {GLFW_KEY_LEFT_CONTROL, HG::Core::Input::Keyboard::Key::LeftCtrl},
        {GLFW_KEY_LEFT_ALT, HG::Core::Input::Keyboard::Key::LeftAlt},
        {GLFW_KEY_LEFT_SUPER, HG::Core::Input::Keyboard::Key::LeftSuper},
        {GLFW_KEY_RIGHT_SHIFT, HG::Core::Input::Keyboard::Key::RightShift},
        {GLFW_KEY_RIGHT_CONTROL, HG::Core::Input::Keyboard::Key::RightCtrl},
        {GLFW_KEY_RIGHT_ALT, HG::Core::Input::Keyboard::Key::RightAlt},
        {GLFW_KEY_RIGHT_SUPER, HG::Core::Input::Keyboard::Key::RightSuper},
        {GLFW_KEY_MENU, HG::Core::Input::Keyboard::Key::Menu}};

    static std::unordered_map<int, HG::Core::Input::Keyboard::Modifiers> modifiers = {
        {GLFW_KEY_LEFT_ALT, HG::Core::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_RIGHT_ALT, HG::Core::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_LEFT_SHIFT, HG::Core::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_RIGHT_SHIFT, HG::Core::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_LEFT_CONTROL, HG::Core::Input::Keyboard::Modifiers::Ctrl},
        {GLFW_KEY_RIGHT_CONTROL, HG::Core::Input::Keyboard::Modifiers::Ctrl},
    };

    {
        auto iter = keys.find(key);

        if (iter != keys.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<HG::Core::Input::Keyboard*>(controller->application()->input()->keyboard())
                    ->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<HG::Core::Input::Keyboard*>(controller->application()->input()->keyboard())
                    ->setPressed(iter->second, false);
            }
        }
    }

    {
        auto iter = modifiers.find(key);

        if (iter != modifiers.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<HG::Core::Input::Keyboard*>(controller->application()->input()->keyboard())
                    ->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<HG::Core::Input::Keyboard*>(controller->application()->input()->keyboard())
                    ->setPressed(iter->second, false);
            }
        }
    }
}

void SystemController::charCallback(GLFWwindow*, unsigned int c)
{
    const_cast<HG::Core::Input::Keyboard*>(controller->application()->input()->keyboard())->addCharacterEntered(c);
}

void SystemController::mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    // No associative container, cause of GLFW buttons are equal to HGEngine's input buttons.
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setPressedButton(static_cast<std::uint8_t>(button), static_cast<bool>(action));
}

void SystemController::mouseWheelCallback(GLFWwindow*, double xDelta, double yDelta)
{
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setMouseWheelScroll(xDelta, yDelta);
}

void SystemController::handleWindowEvents()
{
    const_cast<HG::Core::Input::Window*>(application()->input()->window())
        ->setClosed(static_cast<bool>(glfwWindowShouldClose(m_window)));
}

void SystemController::framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    controller->application()->renderer()->defaultRenderTarget()->setSize({width, height});
}

HG::Utils::Rect SystemController::viewport() const
{
    auto sz = controller->application()->renderer()->defaultRenderTarget()->size();

    return {0, 0, sz.x, sz.y};
}

void SystemController::changeTitle(std::string title)
{
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }
}
} // namespace HG::System::PC
