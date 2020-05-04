// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/Benchmark.hpp>
#include <HG/Core/Input.hpp>
#include <HG/Core/TimeStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/SystemController.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

// ImGui
#include <imgui.h>
#include <imgui_internal.h>

namespace HG::Rendering::Base
{
SystemController::SystemController(HG::Core::Application* application) : m_application(application)
{
}
SystemController::~SystemController()
{
    imGuiDeinit();
    ImGui::DestroyContext();
}
HG::Core::Application* SystemController::application() const
{
    return m_application;
}

bool SystemController::init()
{
    if (!onInit())
    {
        return false;
    }

    ImGui::CreateContext();

    auto& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    return true;
}

void SystemController::deinit()
{
    onDeinit();
    imGuiDeinit();
}
void SystemController::pollEvents()
{
    // Actually poll events
    onPollEvents();

    // ImGui requires render before new frame
    if (application()->scene())
    {
        BENCH("ImGui New Frame");
        imGuiNewFrame();
    }

    imGuiReadKeys();
}

bool SystemController::createWindow(std::uint32_t width, std::uint32_t height, std::string title)
{
    if (!onCreateWindow(width, height, std::move(title)))
    {
        return false;
    }

    HGInfo() << "Initializing ImGui";
    imGuiInit();

    return true;
}

void SystemController::imGuiInit()
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |=
        ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos requests (optional, rarely used)

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.admin
    io.KeyMap[ImGuiKey_Tab]        = int(HG::Core::Input::Keyboard::Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow]  = int(HG::Core::Input::Keyboard::Key::ArrowLeft);
    io.KeyMap[ImGuiKey_RightArrow] = int(HG::Core::Input::Keyboard::Key::ArrowRight);
    io.KeyMap[ImGuiKey_UpArrow]    = int(HG::Core::Input::Keyboard::Key::ArrowUp);
    io.KeyMap[ImGuiKey_DownArrow]  = int(HG::Core::Input::Keyboard::Key::ArrowDown);
    io.KeyMap[ImGuiKey_PageUp]     = int(HG::Core::Input::Keyboard::Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown]   = int(HG::Core::Input::Keyboard::Key::PageDown);
    io.KeyMap[ImGuiKey_Home]       = int(HG::Core::Input::Keyboard::Key::Home);
    io.KeyMap[ImGuiKey_End]        = int(HG::Core::Input::Keyboard::Key::End);
    io.KeyMap[ImGuiKey_Insert]     = int(HG::Core::Input::Keyboard::Key::Insert);
    io.KeyMap[ImGuiKey_Delete]     = int(HG::Core::Input::Keyboard::Key::Delete);
    io.KeyMap[ImGuiKey_Backspace]  = int(HG::Core::Input::Keyboard::Key::Backspace);
    io.KeyMap[ImGuiKey_Space]      = int(HG::Core::Input::Keyboard::Key::Space);
    io.KeyMap[ImGuiKey_Enter]      = int(HG::Core::Input::Keyboard::Key::Return);
    io.KeyMap[ImGuiKey_Escape]     = int(HG::Core::Input::Keyboard::Key::ESC);
    io.KeyMap[ImGuiKey_A]          = int(HG::Core::Input::Keyboard::Key::A);
    io.KeyMap[ImGuiKey_C]          = int(HG::Core::Input::Keyboard::Key::C);
    io.KeyMap[ImGuiKey_V]          = int(HG::Core::Input::Keyboard::Key::V);
    io.KeyMap[ImGuiKey_X]          = int(HG::Core::Input::Keyboard::Key::X);
    io.KeyMap[ImGuiKey_Y]          = int(HG::Core::Input::Keyboard::Key::Y);
    io.KeyMap[ImGuiKey_Z]          = int(HG::Core::Input::Keyboard::Key::Z);

    // todo: Reimplement clipboard
    //    io.SetClipboardTextFn = ImGuiSetClipboardText;
    //    io.GetClipboardTextFn = ImGuiGetClipboardText;
    //    io.ClipboardUserData  = m_window;

    // Load cursors
    // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
    // todo: Reimplement cursors in HG::Core::Input
    //    m_mouseCursors[ImGuiMouseCursor_Arrow]      = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_TextInput]  = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_ResizeAll]  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_ResizeNS]   = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_ResizeEW]   = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    //    m_mouseCursors[ImGuiMouseCursor_Hand]       = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}
void SystemController::imGuiDeinit()
{
    // Destroying cursors
    // todo: Reimplement cursors in HG::Core::Input
    //        for (auto&& cursor : m_mouseCursors)
    //        {
    //            if (cursor != nullptr)
    //            {
    //                glfwDestroyCursor(cursor);
    //                cursor = nullptr;
    //            }
    //        }
}
void SystemController::imGuiNewFrame()
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
    if (application()->systemController()->isWindowFocused())
    {
        // Set OS mouse position if requested (only used when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            // todo: Figure out do we actually need this?
            //                glfwSetCursorPos(m_window, io.MousePos.x, io.MousePos.y);
        }
        else
        {
            auto pos    = application()->input()->mouse()->getMousePosition();
            io.MousePos = ImVec2(pos.x, pos.y);
        }
    }

    for (std::uint8_t i = 0; i < 3; ++i)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = application()->input()->mouse()->isPressed(i);
    }

    // Update OS/hardware mouse cursor if ImGui isn't drawing a software cursor
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0 &&
        !application()->input()->mouse()->isCursorDisabled())
    {
        auto cursor = ImGui::GetMouseCursor();

        if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None)
        {
            application()->input()->mouse()->setCursorHidden(true);
        }
        else
        {
            // todo: Reimplement cursors in HG::Core::Input
            //                glfwSetCursor(m_window,
            //                              m_mouseCursors[cursor] ? m_mouseCursors[cursor] : m_mouseCursors[ImGuiMouseCursor_Arrow]);

            application()->input()->mouse()->setCursorHidden(false);
        }
    }

    io.MouseWheel  = application()->input()->mouse()->getMouseWheelScroll().y;
    io.MouseWheelH = application()->input()->mouse()->getMouseWheelScroll().x;

    // todo: Add gamepad navigation

    ImGui::NewFrame();
}

void SystemController::imGuiReadKeys()
{
    // Processing input for ImGui
    auto& io = ImGui::GetIO();

    application()->input()->keyboard()->visitKeys(
        // Pushed
        [&io](HG::Core::Input::Keyboard::Key key) { io.KeysDown[std::size_t(key)] = true; },
        // Pressed
        nullptr,
        // Released
        [&io](HG::Core::Input::Keyboard::Key key) { io.KeysDown[std::size_t(key)] = false; });

    if (application()->input()->keyboard()->isPressed(HG::Core::Input::Keyboard::Key::NumReturn))
    {
        io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::Return)] = true;
    }

    if (application()->input()->keyboard()->isReleased(HG::Core::Input::Keyboard::Key::NumReturn))
    {
        io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::Return)] = false;
    }

    io.KeyCtrl = io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::LeftCtrl)] ||
                 io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::RightCtrl)];
    io.KeyShift = io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::LeftShift)] ||
                  io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::RightShift)];
    io.KeyAlt = io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::LeftAlt)] ||
                io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::RightAlt)];
    io.KeySuper = io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::LeftSuper)] ||
                  io.KeysDown[std::size_t(HG::Core::Input::Keyboard::Key::RightSuper)];

    application()->input()->keyboard()->visitPressedCharacters([&io](std::uint32_t character) {
        if (character > 0 && character < 0x10000)
        {
            io.AddInputCharacter((unsigned short)character);
        }
    });
}
} // namespace HG::Rendering::Base
