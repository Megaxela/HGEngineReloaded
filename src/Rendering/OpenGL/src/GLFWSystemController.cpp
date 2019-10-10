

#ifdef GRAPHICS_USE_GLFW
// HG::Core
#    include <HG/Core/Application.hpp>
#    include <HG/Core/Benchmark.hpp>
#    include <HG/Core/Input.hpp>
#    include <HG/Core/TimeStatistics.hpp>

// HG::Rendering::Base
#    include <HG/Rendering/Base/Camera.hpp>
#    include <HG/Rendering/Base/RenderTarget.hpp>
#    include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#    include <HG/Rendering/OpenGL/GLFWSystemController.hpp>

// ALogger
#    include <CurrentLogger.hpp>

// GLM
#    include <gl/all.hpp>

// ImGui
#    include <imgui.h>

HG::Rendering::OpenGL::GLFWSystemController::GLFWSystemController(HG::Core::Application* application) :
    SystemController(application),
    m_window(nullptr)
{
}

HG::Rendering::OpenGL::GLFWSystemController::~GLFWSystemController()
{
    imGuiDeinit();

    ImGui::DestroyContext();
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

void HG::Rendering::OpenGL::GLFWSystemController::imGuiInit()
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos requests (optional, rarely used)

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown]   = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert]     = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]      = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C]          = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V]          = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X]          = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y]          = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z]          = GLFW_KEY_Z;

    io.SetClipboardTextFn = ImGuiSetClipboardText;
    io.GetClipboardTextFn = ImGuiGetClipboardText;
    io.ClipboardUserData  = m_window;
    // todo: Fix this define
#    ifdef _WIN32
//    io.ImeWindowHandle = glfwGetWin32Window(m_window);
#    endif

    // Load cursors
    // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
    m_mouseCursors[ImGuiMouseCursor_Arrow]      = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_TextInput]  = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_ResizeAll]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_ResizeNS]   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_ResizeEW]   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_mouseCursors[ImGuiMouseCursor_Hand]       = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

void HG::Rendering::OpenGL::GLFWSystemController::imGuiDeinit()
{
    // Destroying cursors
    for (auto&& cursor : m_mouseCursors)
    {
        if (cursor != nullptr)
        {
            glfwDestroyCursor(cursor);
            cursor = nullptr;
        }
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::imGuiNewFrame()
{
    auto& io = ImGui::GetIO();

    // Setup display size
    // todo: Viewport == Framebuffer for now. Fix if it will change.
    auto viewportRect = viewport();

    io.DisplaySize             = ImVec2(viewportRect.w, viewportRect.h);
    io.DisplayFramebufferScale = ImVec2(1, 1);

    // Setup time step
    io.DeltaTime = float(application()->timeStatistics()->lastFrameDeltaTime().count()) / 1000000;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED))
    {
        // Set OS mouse position if requested (only used when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            glfwSetCursorPos(m_window, io.MousePos.x, io.MousePos.y);
        }
        else
        {
            auto pos    = application()->input()->mouse()->getMousePosition();
            io.MousePos = ImVec2(pos.x, pos.y);
        }
    }

    for (uint8_t i = 0; i < 3; ++i)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = application()->input()->mouse()->isPressed(i);
    }

    // Update OS/hardware mouse cursor if ImGui isn't drawing a software cursor
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0 &&
        glfwGetInputMode(m_window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
    {
        auto cursor = ImGui::GetMouseCursor();

        if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None)
        {
            application()->input()->mouse()->setCursorHidden(true);
        }
        else
        {
            glfwSetCursor(m_window,
                          m_mouseCursors[cursor] ? m_mouseCursors[cursor] : m_mouseCursors[ImGuiMouseCursor_Arrow]);

            application()->input()->mouse()->setCursorHidden(false);
        }
    }

    io.MouseWheel  = application()->input()->mouse()->getMouseWheelScroll().y;
    io.MouseWheelH = application()->input()->mouse()->getMouseWheelScroll().x;

    // todo: Add gamepad navigation

    ImGui::NewFrame();
}

bool HG::Rendering::OpenGL::GLFWSystemController::init()
{
    Info() << "Initializing GLFW";

    // Setting error callback
    glfwSetErrorCallback(
        [](int code, const char* description) { ErrorF() << "GLFW Received error #" << code << ", " << description; });

    // Initializing GLFW
    if (!glfwInit())
    {
        return false;
    }

    // Setting OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    ImGui::CreateContext();

    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    return true;
}

// Incognito
namespace
{
HG::Rendering::OpenGL::GLFWSystemController* controller = nullptr;
}

bool HG::Rendering::OpenGL::GLFWSystemController::createWindow(uint32_t width, uint32_t height, std::string title)
{
    Info() << "Creating window " << width << "x" << height << " with title \"" << title << "\"";

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    Info() << "Initializing ImGui";
    imGuiInit();

    controller = this;

    // Setting event callbacks
    glfwSetKeyCallback(m_window, &GLFWSystemController::keyPressCallback);
    glfwSetCursorPosCallback(m_window, &GLFWSystemController::cursorPosCallback);
    glfwSetMouseButtonCallback(m_window, &GLFWSystemController::mouseButtonCallback);
    glfwSetJoystickCallback(&GLFWSystemController::joystickCallback);
    glfwSetFramebufferSizeCallback(m_window, &GLFWSystemController::framebufferSizeCallback);
    glfwSetCharCallback(m_window, &GLFWSystemController::charCallback);
    glfwSetScrollCallback(m_window, &GLFWSystemController::mouseWheelCallback);

    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setCursorDisabledAction([=](bool disable) {
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

    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setCursorHiddenAction([=](bool hidden) {
            if (hidden)
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        });

    glfwMakeContextCurrent(m_window);

    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    GLenum error;
    if ((error = glewInit()) != GLEW_OK)
    {
        Error() << "Can't init GLEW. Error: " << glewGetErrorString(error);
        return false;
    }

#    ifndef DNDEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        Info() << "Turning on OpenGL debug output.";
        gl::set_debug_output_enabled(true);
        gl::set_syncronous_debug_output_enabled(true);

        glDebugMessageCallback(&HG::Rendering::OpenGL::GLFWSystemController::glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#    endif

    // Enabling cull face to clockwise
    gl::set_polygon_face_culling_enabled(true);
    gl::set_cull_face(GL_BACK);
    gl::set_front_face(GL_CW);

    // Enabling depth test
    gl::set_depth_test_enabled(true);
    gl::set_depth_function(GL_LESS);

    // Blending
    gl::set_blending_enabled(true);
    gl::set_blend_function(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setting default render target size
    controller->application()->renderer()->defaultRenderTarget()->setSize({width, height});

    return true;
}

void HG::Rendering::OpenGL::GLFWSystemController::deinit()
{
    imGuiDeinit();
}

void HG::Rendering::OpenGL::GLFWSystemController::closeWindow()
{
    glfwDestroyWindow(m_window);
}

void HG::Rendering::OpenGL::GLFWSystemController::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

void HG::Rendering::OpenGL::GLFWSystemController::pollEvents()
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

    // ImGui requires render before new frame
    if (application()->scene())
    {
        BENCH("ImGui New Frame");
        imGuiNewFrame();
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::handleGamepadsEvents()
{
    for (auto index = GLFW_JOYSTICK_1; index <= GLFW_JOYSTICK_LAST; ++index)
    {
        int present = glfwJoystickPresent(index);

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<uint8_t>(index), static_cast<bool>(present));

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
                ->setGamepadAxisValue(static_cast<uint8_t>(index), static_cast<uint8_t>(i), axes[i]);
        }

        // Buttons
        const auto buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        for (int i = 0; i < count; ++i)
        {
            if (controller->application()->input()->gamepads()->isButtonPressed(static_cast<uint8_t>(index),
                                                                                static_cast<uint8_t>(i)))
            {
                if (buttons[i] == GLFW_RELEASE)
                {
                    const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
                        ->setGamepadButtonValue(static_cast<uint8_t>(index), static_cast<uint8_t>(i), false);
                }
            }
            else
            {
                if (buttons[i] == GLFW_PRESS)
                {
                    const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
                        ->setGamepadButtonValue(static_cast<uint8_t>(index), static_cast<uint8_t>(i), true);
                }
            }
        }
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::joystickCallback(int gamepad, int event)
{
    if (event == GLFW_CONNECTED)
    {
        InfoEx("OpenGL::RendererController") << "Gamepad #" << gamepad << " connected.";

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), true);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        InfoEx("OpenGL::RendererController") << "Gamepad #" << gamepad << " disconnected.";

        const_cast<HG::Core::Input::Gamepads*>(controller->application()->input()->gamepads())
            ->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), false);
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::cursorPosCallback(GLFWwindow*, double x, double y)
{
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setMousePosition(static_cast<int>(x), static_cast<int>(y));
}

void HG::Rendering::OpenGL::GLFWSystemController::keyPressCallback(GLFWwindow*,
                                                                   int key,
                                                                   int scancode,
                                                                   int action,
                                                                   int mods)
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

    // Processing input for ImGui
    auto& io = ImGui::GetIO();

    if (action == GLFW_PRESS)
    {
        io.KeysDown[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        io.KeysDown[key] = false;
    }

    if (key == GLFW_KEY_KP_ENTER)
    {
        if (action == GLFW_PRESS)
        {
            io.KeysDown[GLFW_KEY_ENTER] = true;
        }
        else
        {
            io.KeysDown[GLFW_KEY_ENTER] = false;
        }
    }

    io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void HG::Rendering::OpenGL::GLFWSystemController::charCallback(GLFWwindow*, unsigned int c)
{
    auto& io = ImGui::GetIO();

    if (c > 0 && c < 0x10000)
    {
        io.AddInputCharacter((unsigned short)c);
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    // No associative container, cause of GLFW buttons are equal to HGEngine's input buttons.
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setPressedButton(static_cast<uint8_t>(button), static_cast<bool>(action));
}

void HG::Rendering::OpenGL::GLFWSystemController::mouseWheelCallback(GLFWwindow*, double xDelta, double yDelta)
{
    const_cast<HG::Core::Input::Mouse*>(controller->application()->input()->mouse())
        ->setMouseWheelScroll(xDelta, yDelta);
}

void HG::Rendering::OpenGL::GLFWSystemController::handleWindowEvents()
{
    const_cast<HG::Core::Input::Window*>(controller->application()->input()->window())
        ->setClosed(static_cast<bool>(glfwWindowShouldClose(m_window)));
}

void HG::Rendering::OpenGL::GLFWSystemController::framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    controller->application()->renderer()->defaultRenderTarget()->setSize({width, height});
}

void HG::Rendering::OpenGL::GLFWSystemController::glDebugOutput(GLenum source,
                                                                GLenum type,
                                                                GLuint id,
                                                                GLenum severity,
                                                                GLsizei,
                                                                const GLchar* message,
                                                                const void*)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 3203 || id == 131185 || id == 131218 || id == 131204 || id == 8 ||
        id == 131076 || /* Usage warning */
        id == 22 ||     /* CPU mapping a bisy "streamed data" BO stalled */
        id == 20 ||     /* GTT mapping a busy "miptree" BO stalled */
        id == 14 ||     /* CPU mapping a busy "miptree" BO stalled */
        id == 18 /* CPU mapping a busy "streamed data" BO stalled */)
    {
        return;
    }

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    std::stringstream ss;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        ss << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        ss << "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        ss << "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        ss << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        ss << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        ss << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        ss << "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        ss << "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        ss << "Other";
        break;
    default:
        ss << "Unexpected";
        break;
    }

    ss << " (severity: ";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        ss << "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        ss << "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        ss << "low";
        break;
    default:
        ss << "unexpected";
        break;
    }

    ss << ", id: " << id;

    ss << ") message received from ";

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        ss << "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        ss << "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        ss << "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        ss << "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        ss << "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        ss << "Other";
        break;
    default:
        ss << "Unexpected";
        break;
        ;
    }

    std::string messageCopy(message);

    std::replace(messageCopy.begin(), messageCopy.end(), '\n', ' ');

    ss << ": " << messageCopy;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        ErrorEx("OpenGL::RendererController") << ss.str();
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
    case GL_DEBUG_TYPE_PERFORMANCE:
    case GL_DEBUG_TYPE_MARKER:
        WarningEx("OpenGL::RendererController") << ss.str();
        break;

    case GL_DEBUG_TYPE_PUSH_GROUP:
    case GL_DEBUG_TYPE_POP_GROUP:
    case GL_DEBUG_TYPE_OTHER:
    default:
        InfoEx("OpenGL::RendererController") << ss.str();
        break;
    }
}

HG::Utils::Rect HG::Rendering::OpenGL::GLFWSystemController::viewport() const
{
    auto sz = controller->application()->renderer()->defaultRenderTarget()->size();

    return {0, 0, sz.x, sz.y};
}

void HG::Rendering::OpenGL::GLFWSystemController::changeTitle(std::string title)
{
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }
}

#endif
