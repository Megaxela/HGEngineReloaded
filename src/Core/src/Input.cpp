#include <CurrentLogger.hpp>
#include "Input.hpp"
#include <algorithm>

CORE_MODULE_NS::Input::Keyboard::Keyboard() :
    m_pushed  (std::size_t(Key::Last) - 1, false),
    m_released(std::size_t(Key::Last) - 1, false),
    m_pressed (std::size_t(Key::Last) - 1, false),
    m_pressedModifiers(0),
    m_pressedCharacter(0)
{

}

bool CORE_MODULE_NS::Input::Keyboard::isPressed(CORE_MODULE_NS::Input::Keyboard::Key key) const
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

bool CORE_MODULE_NS::Input::Keyboard::isPushed(CORE_MODULE_NS::Input::Keyboard::Key key) const
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

bool CORE_MODULE_NS::Input::Keyboard::isReleased(CORE_MODULE_NS::Input::Keyboard::Key key) const
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

bool CORE_MODULE_NS::Input::Keyboard::isModifierPressed(CORE_MODULE_NS::Input::Keyboard::Modifiers modifier) const
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

uint32_t CORE_MODULE_NS::Input::Keyboard::pressedCharacter() const
{
    return m_pressedCharacter;
}

void CORE_MODULE_NS::Input::Keyboard::setPressed(CORE_MODULE_NS::Input::Keyboard::Key key, bool pressed)
{
#ifndef NDEBUG
    if (std::size_t(key) >= m_pressed.size())
    {
        Error() << "There is no key with code " << std::size_t(key) << " requested. Doing nothing.";
        return;
    }
#endif

    m_pressed[std::size_t(key)] = pressed;
}

void CORE_MODULE_NS::Input::Keyboard::setPressed(CORE_MODULE_NS::Input::Keyboard::Modifiers modifier, bool pressed)
{
#ifndef NDEBUG
    if (int(modifier) > 7)
    {
        Error() << "There is no modifier with code " << int(modifier) << " requested. Doing nothing.";
        return;
    }
#endif

    m_pressedModifiers |= (1u << uint32_t(modifier));
}

void CORE_MODULE_NS::Input::Keyboard::setCharacterEntered(uint32_t codepoint)
{
    m_pressedCharacter = codepoint;
}

void CORE_MODULE_NS::Input::Keyboard::tick()
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

CORE_MODULE_NS::Input::Mouse::Mouse() :
    m_mousePos(0, 0),
    m_buttonStates()
{

}

bool CORE_MODULE_NS::Input::Mouse::isPressed(CORE_MODULE_NS::Input::Mouse::Buttons button) const
{
    return isPressed(uint8_t(button));
}

bool CORE_MODULE_NS::Input::Mouse::isPressed(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.pressed;
}

bool CORE_MODULE_NS::Input::Mouse::isPushed(CORE_MODULE_NS::Input::Mouse::Buttons button) const
{
    return isPushed(uint8_t(button));
}

bool CORE_MODULE_NS::Input::Mouse::isPushed(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justPushed;
}

bool CORE_MODULE_NS::Input::Mouse::isReleased(CORE_MODULE_NS::Input::Mouse::Buttons button) const
{
    return isReleased(uint8_t(button));
}

bool CORE_MODULE_NS::Input::Mouse::isReleased(uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justReleased;
}

glm::vec2 CORE_MODULE_NS::Input::Mouse::getMousePosition() const
{
    return m_mousePos;
}

void CORE_MODULE_NS::Input::Mouse::setMousePosition(int x, int y)
{
    m_mousePos.x = x;
    m_mousePos.y = y;
}

void CORE_MODULE_NS::Input::Mouse::setPressedButton(uint8_t button, bool pressed)
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

void CORE_MODULE_NS::Input::Mouse::tick()
{
    for (auto&& state : m_buttonStates)
    {
        state.second.justReleased = false;
        state.second.justPushed = false;
    }
}

CORE_MODULE_NS::Input::Gamepads::Gamepads() :
    m_gamepads()
{

}

bool CORE_MODULE_NS::Input::Gamepads::isConnected() const
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

uint8_t CORE_MODULE_NS::Input::Gamepads::numberOfGamepadsConnected() const
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

bool CORE_MODULE_NS::Input::Gamepads::isButtonPressed(uint8_t gamepad, CORE_MODULE_NS::Input::Gamepads::XBoxButtons button) const
{
    return isButtonPressed(gamepad, uint8_t(button));
}

bool CORE_MODULE_NS::Input::Gamepads::isButtonPressed(uint8_t gamepad, uint8_t button) const
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

bool CORE_MODULE_NS::Input::Gamepads::isButtonPushed(uint8_t gamepad, CORE_MODULE_NS::Input::Gamepads::XBoxButtons button) const
{
    return isButtonPushed(gamepad, uint8_t(button));
}

bool CORE_MODULE_NS::Input::Gamepads::isButtonPushed(uint8_t gamepad, uint8_t button) const
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

bool CORE_MODULE_NS::Input::Gamepads::isButtonReleased(uint8_t gamepad, CORE_MODULE_NS::Input::Gamepads::XBoxButtons button) const
{
    return isButtonReleased(gamepad, uint8_t(button));
}

bool CORE_MODULE_NS::Input::Gamepads::isButtonReleased(uint8_t gamepad, uint8_t button) const
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

uint8_t CORE_MODULE_NS::Input::Gamepads::numberOfButtons(uint8_t gamepad) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return 0;
    }

    return gamepadIterator->second.numberOfButtons;
}

float CORE_MODULE_NS::Input::Gamepads::axisValue(uint8_t gamepad, CORE_MODULE_NS::Input::Gamepads::XBoxAxises axis) const
{
    return axisValue(gamepad, uint8_t(axis));
}

float CORE_MODULE_NS::Input::Gamepads::axisValue(uint8_t gamepad, uint8_t axisIndex) const
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

void CORE_MODULE_NS::Input::Gamepads::setIsConnectedGamepad(uint8_t gamepad, bool connected)
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

void CORE_MODULE_NS::Input::Gamepads::setGamepadAxisValue(uint8_t gamepad, uint8_t axis, float value)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        m_gamepads[gamepad] = GamepadData();
        gamepadIterator = m_gamepads.find(gamepad);
    }

    gamepadIterator->second.axises[axis] = value;
}

void CORE_MODULE_NS::Input::Gamepads::setGamepadButtonValue(uint8_t gamepad, uint8_t button, bool pressed)
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

void CORE_MODULE_NS::Input::Gamepads::tick()
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

CORE_MODULE_NS::Input::Window::Window() :
    m_closed(false)
{

}

bool CORE_MODULE_NS::Input::Window::isClosed() const
{
    return m_closed;
}

void CORE_MODULE_NS::Input::Window::setClosed(bool closed)
{
    m_closed = closed;
}

CORE_MODULE_NS::Input::Input() :
    m_keyboard(),
    m_mouse(),
    m_gamepads(),
    m_window()
{

}

const CORE_MODULE_NS::Input::Keyboard *CORE_MODULE_NS::Input::keyboard() const
{
    return &m_keyboard;
}

const CORE_MODULE_NS::Input::Mouse *CORE_MODULE_NS::Input::mouse() const
{
    return &m_mouse;
}

const CORE_MODULE_NS::Input::Gamepads *CORE_MODULE_NS::Input::gamepads() const
{
    return &m_gamepads;
}

const CORE_MODULE_NS::Input::Window* CORE_MODULE_NS::Input::window() const
{
    return &m_window;
}

void CORE_MODULE_NS::Input::tickControllers()
{
    m_keyboard.tick();
    m_mouse.tick();
    m_gamepads.tick();
}
