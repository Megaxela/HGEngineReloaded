#include <CurrentLogger.hpp>
#include <Input.hpp>
#include <Application.hpp>
#include <Camera.hpp>
#include <gl/all.hpp>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include "GLFWSystemController.hpp"

#ifdef GRAPHICS_USE_GLFW

HG::Rendering::OpenGL::GLFWSystemController::GLFWSystemController(::HG::Core::Application* application) :
    SystemController(application),
    m_window(nullptr)
{

}

HG::Rendering::OpenGL::GLFWSystemController::~GLFWSystemController()
{
    ImGui::DestroyContext();
    glfwTerminate();
    m_window = nullptr;
}

bool HG::Rendering::OpenGL::GLFWSystemController::init()
{
    Info() << "Initializing GLFW";

    // Setting error callback
    glfwSetErrorCallback(
        [](int code, const char* description)
        {
            ErrorF() << "GLFW Received error #" << code << ", " << description;
        }
    );

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

    ImGui_ImplGlfwGL3_Init(m_window, true);

    controller = this;

    // Setting event callbacks
    glfwSetKeyCallback            (m_window, &GLFWSystemController::keyPressCallback);
    glfwSetCursorPosCallback      (m_window, &GLFWSystemController::cursorPosCallback);
    glfwSetMouseButtonCallback    (m_window, &GLFWSystemController::mouseButtonCallback);
    glfwSetJoystickCallback       (          &GLFWSystemController::joystickCallback);
    glfwSetFramebufferSizeCallback(m_window, &GLFWSystemController::framebufferSizeCallback);

    const_cast<::HG::Core::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setCursorDisabledAction(
        [=](bool disable)
        {
            if (disable)
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                // todo: Add checking is it hidden and hide cursor
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    );

    const_cast<::HG::Core::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setCursorHiddenAction(
        [=](bool hidden)
        {
            if (hidden)
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    );

    // todo: Add char callback
//    glfwSetCharCallback(m_window, []());

    glfwMakeContextCurrent(m_window);

    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    GLenum error;
    if ((error = glewInit()) != GLEW_OK)
    {
        Error() << "Can't init GLEW. Error: " << glewGetErrorString(error);
        return false;
    }

#ifndef DNDEBUG
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        Info() << "Turning on OpenGL debug output.";
        gl::set_debug_output_enabled(true);
        gl::set_syncronous_debug_output_enabled(true);

        glDebugMessageCallback(&::HG::Rendering::OpenGL::GLFWSystemController::glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    // Enabling cull face to clockwise
    gl::set_polygon_face_culling_enabled(true);
    gl::set_cull_face(GL_BACK);
    gl::set_front_face(GL_CW);

    // Enabling depth test
    gl::set_depth_test_enabled(true);
    gl::set_depth_function(GL_LESS);

    // Setting viewport
    gl::set_viewport({0, 0}, {static_cast<GLsizei>(width), static_cast<GLsizei>(height)});

    return true;
}

void HG::Rendering::OpenGL::GLFWSystemController::swapBuffers()
{
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_window);
}

void HG::Rendering::OpenGL::GLFWSystemController::pollEvents()
{
    glfwPollEvents();
    handleGamepadsEvents();
    handleWindowEvents();
    ImGui_ImplGlfwGL3_NewFrame();
}

void HG::Rendering::OpenGL::GLFWSystemController::handleGamepadsEvents()
{
    for (auto index = GLFW_JOYSTICK_1; index <= GLFW_JOYSTICK_LAST; ++index)
    {
        int present = glfwJoystickPresent(index);

        const_cast<::HG::Core::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(
            static_cast<uint8_t>(index),
            static_cast<bool>(present)
        );

        if (!present)
        {
            continue;
        }

        int count;

        // Axes
        const float* axes = glfwGetJoystickAxes(index, &count);

        for (int i = 0; i < count; ++i)
        {
            const_cast<::HG::Core::Input::Gamepads*>(
                controller->application()->input()->gamepads()
            )->setGamepadAxisValue(
                static_cast<uint8_t>(index),
                static_cast<uint8_t>(i),
                axes[i]
            );
        }

        // Buttons
        const auto buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

        for (int i = 0; i < count; ++i)
        {
            if (controller->application()
                          ->input()
                          ->gamepads()
                          ->isButtonPressed(
                              static_cast<uint8_t>(index),
                              static_cast<uint8_t>(i)
                          ))
            {
                if (buttons[i] == GLFW_RELEASE)
                {
                    const_cast<::HG::Core::Input::Gamepads*>(
                        controller->application()->input()->gamepads()
                    )->setGamepadButtonValue(
                        static_cast<uint8_t>(index),
                        static_cast<uint8_t>(i),
                        false
                    );
                }
            }
            else
            {
                if (buttons[i] == GLFW_PRESS)
                {
                    const_cast<::HG::Core::Input::Gamepads*>(
                        controller->application()->input()->gamepads()
                    )->setGamepadButtonValue(
                        static_cast<uint8_t>(index),
                        static_cast<uint8_t>(i),
                        true
                    );
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

        const_cast<::HG::Core::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), true);
    }
    else if (event == GLFW_DISCONNECTED)
    {
        InfoEx("OpenGL::RendererController") << "Gamepad #" << gamepad << " disconnected.";

        const_cast<::HG::Core::Input::Gamepads*>(
            controller->application()->input()->gamepads()
        )->setIsConnectedGamepad(static_cast<uint8_t>(gamepad), false);
    }
}

void HG::Rendering::OpenGL::GLFWSystemController::cursorPosCallback(GLFWwindow*, double x, double y)
{
    const_cast<::HG::Core::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setMousePosition(
        static_cast<int>(x),
        static_cast<int>(y)
    );
}

void HG::Rendering::OpenGL::GLFWSystemController::keyPressCallback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    static std::map<int, ::HG::Core::Input::Keyboard::Key> keys = {
        {GLFW_KEY_0,                ::HG::Core::Input::Keyboard::Key::N0},
        {GLFW_KEY_1,                ::HG::Core::Input::Keyboard::Key::N1},
        {GLFW_KEY_2,                ::HG::Core::Input::Keyboard::Key::N2},
        {GLFW_KEY_3,                ::HG::Core::Input::Keyboard::Key::N3},
        {GLFW_KEY_4,                ::HG::Core::Input::Keyboard::Key::N4},
        {GLFW_KEY_5,                ::HG::Core::Input::Keyboard::Key::N5},
        {GLFW_KEY_6,                ::HG::Core::Input::Keyboard::Key::N6},
        {GLFW_KEY_7,                ::HG::Core::Input::Keyboard::Key::N7},
        {GLFW_KEY_8,                ::HG::Core::Input::Keyboard::Key::N8},
        {GLFW_KEY_9,                ::HG::Core::Input::Keyboard::Key::N9},
        {GLFW_KEY_SPACE,            ::HG::Core::Input::Keyboard::Key::Space},
        {GLFW_KEY_APOSTROPHE,       ::HG::Core::Input::Keyboard::Key::SingleQuotation},
        {GLFW_KEY_COMMA,            ::HG::Core::Input::Keyboard::Key::Comma},
        {GLFW_KEY_MINUS,            ::HG::Core::Input::Keyboard::Key::Minus},
        {GLFW_KEY_PERIOD,           ::HG::Core::Input::Keyboard::Key::Dot},
        {GLFW_KEY_SLASH,            ::HG::Core::Input::Keyboard::Key::Slash},
        {GLFW_KEY_SEMICOLON,        ::HG::Core::Input::Keyboard::Key::Semicolon},
        {GLFW_KEY_EQUAL,            ::HG::Core::Input::Keyboard::Key::Equal},
        {GLFW_KEY_A,                ::HG::Core::Input::Keyboard::Key::A},
        {GLFW_KEY_B,                ::HG::Core::Input::Keyboard::Key::B},
        {GLFW_KEY_C,                ::HG::Core::Input::Keyboard::Key::C},
        {GLFW_KEY_D,                ::HG::Core::Input::Keyboard::Key::D},
        {GLFW_KEY_E,                ::HG::Core::Input::Keyboard::Key::E},
        {GLFW_KEY_F,                ::HG::Core::Input::Keyboard::Key::F},
        {GLFW_KEY_G,                ::HG::Core::Input::Keyboard::Key::G},
        {GLFW_KEY_H,                ::HG::Core::Input::Keyboard::Key::H},
        {GLFW_KEY_I,                ::HG::Core::Input::Keyboard::Key::I},
        {GLFW_KEY_J,                ::HG::Core::Input::Keyboard::Key::J},
        {GLFW_KEY_K,                ::HG::Core::Input::Keyboard::Key::K},
        {GLFW_KEY_L,                ::HG::Core::Input::Keyboard::Key::L},
        {GLFW_KEY_M,                ::HG::Core::Input::Keyboard::Key::M},
        {GLFW_KEY_N,                ::HG::Core::Input::Keyboard::Key::N},
        {GLFW_KEY_O,                ::HG::Core::Input::Keyboard::Key::O},
        {GLFW_KEY_P,                ::HG::Core::Input::Keyboard::Key::P},
        {GLFW_KEY_Q,                ::HG::Core::Input::Keyboard::Key::Q},
        {GLFW_KEY_R,                ::HG::Core::Input::Keyboard::Key::R},
        {GLFW_KEY_S,                ::HG::Core::Input::Keyboard::Key::S},
        {GLFW_KEY_T,                ::HG::Core::Input::Keyboard::Key::T},
        {GLFW_KEY_U,                ::HG::Core::Input::Keyboard::Key::U},
        {GLFW_KEY_V,                ::HG::Core::Input::Keyboard::Key::V},
        {GLFW_KEY_W,                ::HG::Core::Input::Keyboard::Key::W},
        {GLFW_KEY_X,                ::HG::Core::Input::Keyboard::Key::X},
        {GLFW_KEY_Y,                ::HG::Core::Input::Keyboard::Key::Y},
        {GLFW_KEY_Z,                ::HG::Core::Input::Keyboard::Key::Z},
        {GLFW_KEY_LEFT_BRACKET,     ::HG::Core::Input::Keyboard::Key::SquareBracketOpen},
        {GLFW_KEY_BACKSLASH,        ::HG::Core::Input::Keyboard::Key::Backslash},
        {GLFW_KEY_RIGHT_BRACKET,    ::HG::Core::Input::Keyboard::Key::SquareBracketClose},
        {GLFW_KEY_GRAVE_ACCENT,     ::HG::Core::Input::Keyboard::Key::Tilda}, // todo: Add world keys here
        {GLFW_KEY_ESCAPE,           ::HG::Core::Input::Keyboard::Key::ESC},
        {GLFW_KEY_ENTER,            ::HG::Core::Input::Keyboard::Key::Return},
        {GLFW_KEY_TAB,              ::HG::Core::Input::Keyboard::Key::Tab},
        {GLFW_KEY_BACKSPACE,        ::HG::Core::Input::Keyboard::Key::Backspace},
        {GLFW_KEY_INSERT,           ::HG::Core::Input::Keyboard::Key::Insert},
        {GLFW_KEY_DELETE,           ::HG::Core::Input::Keyboard::Key::Delete},
        {GLFW_KEY_RIGHT,            ::HG::Core::Input::Keyboard::Key::ArrowRight},
        {GLFW_KEY_LEFT,             ::HG::Core::Input::Keyboard::Key::ArrowLeft},
        {GLFW_KEY_DOWN,             ::HG::Core::Input::Keyboard::Key::ArrowDown},
        {GLFW_KEY_UP,               ::HG::Core::Input::Keyboard::Key::ArrowUp},
        {GLFW_KEY_PAGE_UP,          ::HG::Core::Input::Keyboard::Key::PageUp},
        {GLFW_KEY_PAGE_DOWN,        ::HG::Core::Input::Keyboard::Key::PageDown},
        {GLFW_KEY_HOME,             ::HG::Core::Input::Keyboard::Key::Home},
        {GLFW_KEY_END,              ::HG::Core::Input::Keyboard::Key::End},
        {GLFW_KEY_CAPS_LOCK,        ::HG::Core::Input::Keyboard::Key::CapsLock},
        {GLFW_KEY_NUM_LOCK,         ::HG::Core::Input::Keyboard::Key::NumLock},
        {GLFW_KEY_PRINT_SCREEN,     ::HG::Core::Input::Keyboard::Key::PrintScreen},
        {GLFW_KEY_PAUSE,            ::HG::Core::Input::Keyboard::Key::Pause},
        {GLFW_KEY_F1,               ::HG::Core::Input::Keyboard::Key::F1},
        {GLFW_KEY_F2,               ::HG::Core::Input::Keyboard::Key::F2},
        {GLFW_KEY_F3,               ::HG::Core::Input::Keyboard::Key::F3},
        {GLFW_KEY_F4,               ::HG::Core::Input::Keyboard::Key::F4},
        {GLFW_KEY_F5,               ::HG::Core::Input::Keyboard::Key::F5},
        {GLFW_KEY_F6,               ::HG::Core::Input::Keyboard::Key::F6},
        {GLFW_KEY_F7,               ::HG::Core::Input::Keyboard::Key::F7},
        {GLFW_KEY_F8,               ::HG::Core::Input::Keyboard::Key::F8},
        {GLFW_KEY_F9,               ::HG::Core::Input::Keyboard::Key::F9},
        {GLFW_KEY_F10,              ::HG::Core::Input::Keyboard::Key::F10},
        {GLFW_KEY_F11,              ::HG::Core::Input::Keyboard::Key::F11},
        {GLFW_KEY_F12,              ::HG::Core::Input::Keyboard::Key::F12},
        {GLFW_KEY_F13,              ::HG::Core::Input::Keyboard::Key::F13},
        {GLFW_KEY_F14,              ::HG::Core::Input::Keyboard::Key::F14},
        {GLFW_KEY_F15,              ::HG::Core::Input::Keyboard::Key::F15},
        {GLFW_KEY_F16,              ::HG::Core::Input::Keyboard::Key::F16},
        {GLFW_KEY_F17,              ::HG::Core::Input::Keyboard::Key::F17},
        {GLFW_KEY_F18,              ::HG::Core::Input::Keyboard::Key::F18},
        {GLFW_KEY_F19,              ::HG::Core::Input::Keyboard::Key::F19},
        {GLFW_KEY_F20,              ::HG::Core::Input::Keyboard::Key::F20},
        {GLFW_KEY_F21,              ::HG::Core::Input::Keyboard::Key::F21},
        {GLFW_KEY_F22,              ::HG::Core::Input::Keyboard::Key::F22},
        {GLFW_KEY_F23,              ::HG::Core::Input::Keyboard::Key::F23},
        {GLFW_KEY_F24,              ::HG::Core::Input::Keyboard::Key::F24},
        {GLFW_KEY_F25,              ::HG::Core::Input::Keyboard::Key::F25},
        {GLFW_KEY_KP_0,             ::HG::Core::Input::Keyboard::Key::Num0},
        {GLFW_KEY_KP_1,             ::HG::Core::Input::Keyboard::Key::Num1},
        {GLFW_KEY_KP_2,             ::HG::Core::Input::Keyboard::Key::Num2},
        {GLFW_KEY_KP_3,             ::HG::Core::Input::Keyboard::Key::Num3},
        {GLFW_KEY_KP_4,             ::HG::Core::Input::Keyboard::Key::Num4},
        {GLFW_KEY_KP_5,             ::HG::Core::Input::Keyboard::Key::Num5},
        {GLFW_KEY_KP_6,             ::HG::Core::Input::Keyboard::Key::Num6},
        {GLFW_KEY_KP_7,             ::HG::Core::Input::Keyboard::Key::Num7},
        {GLFW_KEY_KP_8,             ::HG::Core::Input::Keyboard::Key::Num8},
        {GLFW_KEY_KP_9,             ::HG::Core::Input::Keyboard::Key::Num9},
        {GLFW_KEY_KP_DECIMAL,       ::HG::Core::Input::Keyboard::Key::NumDot},
        {GLFW_KEY_KP_DIVIDE,        ::HG::Core::Input::Keyboard::Key::NumDivide},
        {GLFW_KEY_KP_MULTIPLY,      ::HG::Core::Input::Keyboard::Key::NumMultiply},
        {GLFW_KEY_KP_SUBTRACT,      ::HG::Core::Input::Keyboard::Key::NumSubstract},
        {GLFW_KEY_KP_ADD,           ::HG::Core::Input::Keyboard::Key::NumAdd},
        {GLFW_KEY_KP_ENTER,         ::HG::Core::Input::Keyboard::Key::NumReturn},
        {GLFW_KEY_KP_EQUAL,         ::HG::Core::Input::Keyboard::Key::NumEqual},
        {GLFW_KEY_LEFT_SHIFT,       ::HG::Core::Input::Keyboard::Key::LeftShift},
        {GLFW_KEY_LEFT_CONTROL,     ::HG::Core::Input::Keyboard::Key::LeftCtrl},
        {GLFW_KEY_LEFT_ALT,         ::HG::Core::Input::Keyboard::Key::LeftAlt},
        {GLFW_KEY_LEFT_SUPER,       ::HG::Core::Input::Keyboard::Key::Super},
        {GLFW_KEY_RIGHT_SHIFT,      ::HG::Core::Input::Keyboard::Key::RightShift},
        {GLFW_KEY_RIGHT_CONTROL,    ::HG::Core::Input::Keyboard::Key::RightCtrl},
        {GLFW_KEY_RIGHT_ALT,        ::HG::Core::Input::Keyboard::Key::RightAlt},
        {GLFW_KEY_MENU,             ::HG::Core::Input::Keyboard::Key::Menu}
    };

    static std::map<int, ::HG::Core::Input::Keyboard::Modifiers> modifiers = {
        {GLFW_KEY_LEFT_ALT,         ::HG::Core::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_RIGHT_ALT,        ::HG::Core::Input::Keyboard::Modifiers::Alt},
        {GLFW_KEY_LEFT_SHIFT,       ::HG::Core::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_RIGHT_SHIFT,      ::HG::Core::Input::Keyboard::Modifiers::Shift},
        {GLFW_KEY_LEFT_CONTROL,     ::HG::Core::Input::Keyboard::Modifiers::Ctrl},
        {GLFW_KEY_RIGHT_CONTROL,    ::HG::Core::Input::Keyboard::Modifiers::Ctrl},
    };

    {
        auto iter = keys.find(key);

        if (iter != keys.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<::HG::Core::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<::HG::Core::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, false);
            }

        }
    }

    {
        auto iter = modifiers.find(key);

        if (iter != modifiers.end())
        {
            if (action == GLFW_PRESS)
            {
                const_cast<::HG::Core::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, true);
            }
            else if (action == GLFW_RELEASE)
            {
                const_cast<::HG::Core::Input::Keyboard*>(
                    controller->application()->input()->keyboard()
                )->setPressed(iter->second, false);
            }
        }
    }

    ImGui_ImplGlfw_KeyCallback(nullptr, key, scancode, action, mods);
}

void HG::Rendering::OpenGL::GLFWSystemController::mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    const_cast<::HG::Core::Input::Mouse*>(
        controller->application()->input()->mouse()
    )->setPressedButton(
        static_cast<uint8_t>(button),
        static_cast<bool>(action)
    );
}

void HG::Rendering::OpenGL::GLFWSystemController::handleWindowEvents()
{
    const_cast<::HG::Core::Input::Window*>(
        controller->application()->input()->window()
    )->setClosed(static_cast<bool>(glfwWindowShouldClose(m_window)));
}

void HG::Rendering::OpenGL::GLFWSystemController::framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    gl::set_viewport({0, 0}, {width, height});

    if (::HG::Rendering::Base::Camera::active())
    {
        ::HG::Rendering::Base::Camera::active()->setViewport(0, 0, width, height);
    }

}

void HG::Rendering::OpenGL::GLFWSystemController::glDebugOutput(GLenum source,
                                                                GLenum type,
                                                                GLuint id,
                                                                GLenum severity,
                                                                GLsizei,
                                                                const GLchar *message,
                                                                const void *)
{
    // ignore non-significant error/warning codes
    if(id == 131169 ||
       id == 3203   ||
       id == 131185 ||
       id == 131218 ||
       id == 131204 ||
       id == 8 ||
       id == 22 || /* CPU mapping a bisy "streamed data" BO stalled */
       id == 20 || /* GTT mapping a busy "miptree" BO stalled */
       id == 14 || /* CPU mapping a busy "miptree" BO stalled */
       id == 18    /* CPU mapping a busy "streamed data" BO stalled */)
    {
        return;
    }

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    std::stringstream ss;
    ss << "Debug message (" << id << "): " <<  message << std::endl;

    ss << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             ss << "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     ss << "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           ss << "Other"; break;
    default:                              ss << "Unexpected"; break;;
    } ss << std::endl;

    ss << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               ss << "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         ss << "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         ss << "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              ss << "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           ss << "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               ss << "Other"; break;
    default:                                ss << "Unexpected"; break;
    } ss << std::endl;

    ss << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         ss << "high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       ss << "medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          ss << "low"; break;
    default:                             ss << "unexpected"; break;
    } ss << std::endl;
    ss << std::endl;

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        ErrorEx("OpenGL::RendererController")
            << "OpenGL Error:\n" << ss.str();
    }
    else
    {
        WarningEx("OpenGL::RendererController")
            << "OpenGL Warning:\n" << ss.str();
    }
}

HG::Utils::Rect HG::Rendering::OpenGL::GLFWSystemController::viewport() const
{
    ::HG::Utils::Rect rect;

    glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(&rect));

    return rect;
}

#endif