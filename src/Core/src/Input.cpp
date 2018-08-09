#include <CurrentLogger.hpp>
#include "Input.hpp"
#include <algorithm>

HG::Core::Input::Keyboard::Keyboard() :
    m_pushed  (std::size_t(Key::Last) - 1, false),
    m_released(std::size_t(Key::Last) - 1, false),
    m_pressed (std::size_t(Key::Last) - 1, false),
    m_pressedModifiers(0),
    m_pressedCharacter(0)
{

}

bool HG::Core::Input::Keyboard::isPressed(HG::Core::Input::Keyboard::Key key) const
{
#ifndef NDEBUG
    if (std::size_t(key) >= m_pressed.size())
    {
        Error() << "There is no key with code " << std::size_t(key) << " requested. Returning false.";
        return false;
    }
#endif

    return m_pressed[std::size_t(key)];
}

bool HG::Core::Input::Keyboard::isPushed(HG::Core::Input::Keyboard::Key key) const
{
#ifndef NDEBUG
    if (std::size_t(key) >= m_pushed.size())
    {
        Error() << "There is no key with code " << std::size_t(key) << " requested. Returning false.";
        return false;
    }
#endif

    return m_pushed[std::size_t(key)];
}

bool HG::Core::Input::Keyboard::isReleased(HG::Core::Input::Keyboard::Key key) const
{
#ifndef NDEBUG
    if (std::size_t(key) >= m_released.size())
    {
        Error() << "There is no key with code " << std::size_t(key) << " requested. Returning false.";
        return false;
    }
#endif

    return m_released[std::size_t(key)];
}

bool HG::Core::Input::Keyboard::isModifierPressed(HG::Core::Input::Keyboard::Modifiers modifier) const
{
#ifndef NDEBUG
    if (int(modifier) > 7)
    {
        Error() << "There is no modifier with code " << int(modifier) << " requested. Returning false.";
        return false;
    }
#endif

    return static_cast<bool>(m_pressedModifiers & (1u << int(modifier)));
}

uint32_t HG::Core::Input::Keyboard::pressedCharacter() const
{
    return m_pressedCharacter;
}

void HG::Core::Input::Keyboard::setPressed(HG::Core::Input::Keyboard::Key key, bool pressed)
{
#ifndef NDEBUG
    if (std::size_t(key) >= m_pressed.size())
    {
        Error() << "There is no key with code " << std::size_t(key) << " requested. Doing nothing.";
        return;
    }
#endif


    if (pressed)
    {
        m_pushed[std::size_t(key)] = true;
        m_pressed[std::size_t(key)] = true;
    }
    else
    {
        m_released[std::size_t(key)]= true;
        m_pressed[std::size_t(key)] = false;
    }
}

void HG::Core::Input::Keyboard::setPressed(HG::Core::Input::Keyboard::Modifiers modifier, bool pressed)
{
#ifndef NDEBUG
    if (int(modifier) > 7)
    {
        Error() << "There is no modifier with code " << int(modifier) << " requested. Doing nothing.";
        return;
    }
#endif

    if (pressed)
    {
        m_pressedModifiers |= (1u << uint32_t(modifier));
    }
    else
    {
        m_pressedModifiers &= ~(1u << uint32_t(modifier));
    }
}

void HG::Core::Input::Keyboard::setCharacterEntered(uint32_t codepoint)
{
    m_pressedCharacter = codepoint;
}

void HG::Core::Input::Keyboard::tick()
{
    for (auto&& el : m_pushed)
    {
        el = false;
    }

    for (auto&& el : m_released)
    {
        el = false;
    }

    m_pressedCharacter = 0;
}

HG::Core::Input::Mouse::Mouse() :
    m_mousePos(0, 0),
    m_buttonStates()
{

}

bool HG::Core::Input::Mouse::isPressed(HG::Core::Input::Mouse::Buttons button) const
{
    return isPressed(uint8_t(button));
}

bool HG::Core::Input::Mouse::isPressed(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.pressed;
}

bool HG::Core::Input::Mouse::isPushed(HG::Core::Input::Mouse::Buttons button) const
{
    return isPushed(uint8_t(button));
}

bool HG::Core::Input::Mouse::isPushed(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justPushed;
}

bool HG::Core::Input::Mouse::isReleased(HG::Core::Input::Mouse::Buttons button) const
{
    return isReleased(uint8_t(button));
}

bool HG::Core::Input::Mouse::isReleased(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justReleased;
}

glm::vec2 HG::Core::Input::Mouse::getMousePosition() const
{
    return m_mousePos;
}

void HG::Core::Input::Mouse::setCursorDisabled(bool locked) const
{
    if (m_disabledAction == nullptr)
    {
        Error() << "Can't disable cursor, in case of unavailable action.";
        return;
    }
        
    m_disabledAction(locked);
}

void HG::Core::Input::Mouse::setCursorHidden(bool hidden) const
{
    if (m_hiddenAction == nullptr)
    {
        Error() << "Can't hide cursor, in case of unavailable action.";
        return;
    }
    
    m_hiddenAction(hidden);
}

void HG::Core::Input::Mouse::setCursorDisabledAction(std::function<void(bool)> disabledAction)
{
    m_disabledAction = std::move(disabledAction);
}

void HG::Core::Input::Mouse::setCursorHiddenAction(std::function<void(bool)> hiddenAction)
{
    m_hiddenAction = std::move(hiddenAction);
}

void HG::Core::Input::Mouse::setMousePosition(int x, int y)
{
    m_mousePos.x = x;
    m_mousePos.y = y;
}

void HG::Core::Input::Mouse::setPressedButton(uint8_t button, bool pressed)
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        iter = m_buttonStates.insert_or_assign(button, ButtonState()).first;
    }

    if (pressed)
    {
        iter->second.justPushed = true;
    }
    else
    {
        iter->second.justReleased = false;
    }

    iter->second.pressed = pressed;
}

void HG::Core::Input::Mouse::tick()
{
    for (auto&& state : m_buttonStates)
    {
        state.second.justReleased = false;
        state.second.justPushed = false;
    }
}

HG::Core::Input::Gamepads::Gamepads() :
    m_gamepads()
{

}

bool HG::Core::Input::Gamepads::isConnected() const
{
    for (auto&& gamepad : m_gamepads)
    {
        if (gamepad.second.connected)
        {
            return true;
        }
    }

    return true;
}

uint8_t HG::Core::Input::Gamepads::numberOfGamepadsConnected() const
{
    uint8_t numberOfGamepads = 0;

    for (auto gamepad : m_gamepads)
    {
        if (gamepad.second.connected)
        {
            ++numberOfGamepads;
        }
    }

    return numberOfGamepads;
}

bool HG::Core::Input::Gamepads::isButtonPressed(uint8_t gamepad, HG::Core::Input::Gamepads::XBoxButtons button) const
{
    return isButtonPressed(gamepad, uint8_t(button));
}

bool HG::Core::Input::Gamepads::isButtonPressed(uint8_t gamepad, uint8_t button) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return false;
    }

    auto buttonIterator = gamepadIterator->second.buttons.find(button);

    if (buttonIterator == gamepadIterator->second.buttons.end())
    {
        return false;
    }

    return buttonIterator->second.pressed;
}

bool HG::Core::Input::Gamepads::isButtonPushed(uint8_t gamepad, HG::Core::Input::Gamepads::XBoxButtons button) const
{
    return isButtonPushed(gamepad, uint8_t(button));
}

bool HG::Core::Input::Gamepads::isButtonPushed(uint8_t gamepad, uint8_t button) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return false;
    }

    auto buttonIterator = gamepadIterator->second.buttons.find(button);

    if (buttonIterator == gamepadIterator->second.buttons.end())
    {
        return false;
    }

    return buttonIterator->second.justPushed;
}

bool HG::Core::Input::Gamepads::isButtonReleased(uint8_t gamepad, HG::Core::Input::Gamepads::XBoxButtons button) const
{
    return isButtonReleased(gamepad, uint8_t(button));
}

bool HG::Core::Input::Gamepads::isButtonReleased(uint8_t gamepad, uint8_t button) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return false;
    }

    auto buttonIterator = gamepadIterator->second.buttons.find(button);

    if (buttonIterator == gamepadIterator->second.buttons.end())
    {
        return false;
    }

    return buttonIterator->second.justReleased;
}

uint8_t HG::Core::Input::Gamepads::numberOfButtons(uint8_t gamepad) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return 0;
    }

    return gamepadIterator->second.numberOfButtons;
}

float HG::Core::Input::Gamepads::axisValue(uint8_t gamepad, HG::Core::Input::Gamepads::XBoxAxises axis) const
{
    return axisValue(gamepad, uint8_t(axis));
}

float HG::Core::Input::Gamepads::axisValue(uint8_t gamepad, uint8_t axisIndex) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return 0.0f;
    }

    auto buttonIterator = gamepadIterator->second.axises.find(axisIndex);

    if (buttonIterator == gamepadIterator->second.axises.end())
    {
        return 0.0f;
    }

    return buttonIterator->second;
}

void HG::Core::Input::Gamepads::setIsConnectedGamepad(uint8_t gamepad, bool connected)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator != m_gamepads.end())
    {
        gamepadIterator->second.connected = connected;
    }
    else
    {
        m_gamepads[gamepad] = GamepadData();
        m_gamepads[gamepad].connected = connected;
    }
}

void HG::Core::Input::Gamepads::setGamepadAxisValue(uint8_t gamepad, uint8_t axis, float value)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        m_gamepads[gamepad] = GamepadData();
        gamepadIterator = m_gamepads.find(gamepad);
    }

    gamepadIterator->second.axises[axis] = value;
}

void HG::Core::Input::Gamepads::setGamepadButtonValue(uint8_t gamepad, uint8_t button, bool pressed)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        m_gamepads[gamepad] = GamepadData();
        gamepadIterator = m_gamepads.find(gamepad);
    }

    if (pressed)
    {
        gamepadIterator->second.buttons[button].justPushed = true;
        gamepadIterator->second.buttons[button].pressed = true;
    }
    else
    {
        gamepadIterator->second.buttons[button].justReleased = true;
    }
}

void HG::Core::Input::Gamepads::tick()
{
    for (auto& m_gamepad : m_gamepads)
    {
        for (auto& button : m_gamepad.second.buttons)
        {
            button.second.justPushed = false;
            button.second.justReleased = false;
        }
    }
}

HG::Core::Input::Window::Window() :
    m_closed(false)
{

}

bool HG::Core::Input::Window::isClosed() const
{
    return m_closed;
}

void HG::Core::Input::Window::setClosed(bool closed)
{
    m_closed = closed;
}

HG::Core::Input::Input() :
    m_keyboard(),
    m_mouse(),
    m_gamepads(),
    m_window()
{

}

const HG::Core::Input::Keyboard *HG::Core::Input::keyboard() const
{
    return &m_keyboard;
}

const HG::Core::Input::Mouse *HG::Core::Input::mouse() const
{
    return &m_mouse;
}

const HG::Core::Input::Gamepads *HG::Core::Input::gamepads() const
{
    return &m_gamepads;
}

const HG::Core::Input::Window* HG::Core::Input::window() const
{
    return &m_window;
}

void HG::Core::Input::tickControllers()
{
    m_keyboard.tick();
    m_mouse.tick();
    m_gamepads.tick();
}
