// C++ STL
#include <algorithm>

// HG::Core
#include <HG/Core/BuildProperties.hpp>
#include <HG/Core/Input.hpp>

// HG::Utils
#include <HG/Utils/Logging.hpp>

namespace HG::Core
{
Input::Keyboard::Keyboard() :
    m_pushed(std::size_t(Key::Last) + 1, false),
    m_released(std::size_t(Key::Last) + 1, false),
    m_pressed(std::size_t(Key::Last) + 1, false),
    m_pressedModifiers(0),
    m_pressedCharacters(),
    m_numberOfPressedCharacters(0)
{
}
void Input::Keyboard::visitKeys(const std::function<void(Key)>& pushedCallback,
                                const std::function<void(Key)>& pressedCallback,
                                const std::function<void(Key)>& releasedCallback) const
{
    for (auto iter = std::size_t(Key::First); iter <= std::size_t(Key::Last); ++iter)
    {
        if (m_pushed[iter] && pushedCallback != nullptr)
        {
            pushedCallback(Key(iter));
        }
        if (m_pressed[iter] && pressedCallback != nullptr)
        {
            pressedCallback(Key(iter));
        }
        if (m_released[iter] && releasedCallback != nullptr)
        {
            releasedCallback(Key(iter));
        }
    }
}

void Input::Keyboard::visitPressedModifiers(const std::function<void(Modifiers)>& callback) const
{
    for (auto iter = std::uint8_t(Modifiers::First); iter <= std::uint8_t(Modifiers::Last); ++iter)
    {
        if ((m_pressedModifiers & iter) != 0)
        {
            callback(Modifiers(iter));
        }
    }
}

bool Input::Keyboard::isPressed(Input::Keyboard::Key key) const
{
    if constexpr (BuildProperties::isDebug())
    {
        if (std::size_t(key) >= m_pressed.size())
        {
            HGError("There is no key with code {} requested. Returning false.", std::size_t(key));
            return false;
        }
    }

    return m_pressed[std::size_t(key)];
}

bool Input::Keyboard::isPushed(Input::Keyboard::Key key) const
{
    if constexpr (BuildProperties::isDebug())
    {
        if (std::size_t(key) >= m_pushed.size())
        {
            HGError("There is no key with code {} requested. Returning false.", std::size_t(key));
            return false;
        }
    }

    return m_pushed[std::size_t(key)];
}

bool Input::Keyboard::isReleased(Input::Keyboard::Key key) const
{
    if constexpr (BuildProperties::isDebug())
    {
        if (std::size_t(key) >= m_released.size())
        {
            HGError("There is no key with code {} requested. Returning false.", std::size_t(key));
            return false;
        }
    }

    return m_released[std::size_t(key)];
}

bool Input::Keyboard::isModifierPressed(Input::Keyboard::Modifiers modifier) const
{
    if constexpr (BuildProperties::isDebug())
    {
        if ((int)modifier > 7)
        {
            HGError("There is no modifier with code {} requested. Returning false.", int(modifier));
            return false;
        }
    }

    return static_cast<bool>(m_pressedModifiers & (1u << int(modifier)));
}

void Input::Keyboard::visitPressedCharacters(std::function<void(std::uint32_t)> visitor) const
{
    for (std::uint8_t i = 0; i < m_numberOfPressedCharacters; ++i)
    {
        visitor(m_pressedCharacters[i]);
    }
}

void Input::Keyboard::setPressed(Input::Keyboard::Key key, bool pressed)
{
    if constexpr (BuildProperties::isDebug())
    {
        if (std::size_t(key) >= m_pressed.size())
        {
            HGError("There is no key with code {} requested. Doing nothing.", std::size_t(key));
            return;
        }
    }

    if (pressed)
    {
        m_pushed[std::size_t(key)]  = true;
        m_pressed[std::size_t(key)] = true;
    }
    else
    {
        m_released[std::size_t(key)] = true;
        m_pressed[std::size_t(key)]  = false;
    }
}

void Input::Keyboard::setPressed(Input::Keyboard::Modifiers modifier, bool pressed)
{
    if constexpr (BuildProperties::isDebug())
    {
        if ((int)modifier > 7)
        {
            HGError("There is no modifier with code {} requested. Doing nothing.", int(modifier));
            return;
        }
    }

    if (pressed)
    {
        m_pressedModifiers |= (1u << std::uint32_t(modifier));
    }
    else
    {
        m_pressedModifiers &= ~(1u << std::uint32_t(modifier));
    }
}

void Input::Keyboard::addCharacterEntered(std::uint32_t codepoint)
{
    m_pressedCharacters[m_numberOfPressedCharacters++] = codepoint;
}

void Input::Keyboard::tick()
{
    for (auto&& el : m_pushed)
    {
        el = false;
    }

    for (auto&& el : m_released)
    {
        el = false;
    }

    m_numberOfPressedCharacters = 0;
}

Input::Mouse::Mouse() :
    m_mousePos(0, 0),
    m_mouseWheelScrollDelta(0, 0),
    m_buttonStates(),
    m_disabledAction(),
    m_hiddenAction(),
    m_isDisabledAction(),
    m_isHiddenAction()
{
}

bool Input::Mouse::isPressed(Input::Mouse::Buttons button) const
{
    return isPressed(std::uint8_t(button));
}

bool Input::Mouse::isPressed(std::uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.pressed;
}

bool Input::Mouse::isPushed(Input::Mouse::Buttons button) const
{
    return isPushed(std::uint8_t(button));
}

bool Input::Mouse::isPushed(std::uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justPushed;
}

bool Input::Mouse::isReleased(Input::Mouse::Buttons button) const
{
    return isReleased(std::uint8_t(button));
}

bool Input::Mouse::isReleased(std::uint8_t button) const
{
    auto iter = m_buttonStates.find(button);

    if (iter == m_buttonStates.end())
    {
        return false;
    }

    return iter->second.justReleased;
}

glm::vec2 Input::Mouse::getMousePosition() const
{
    return m_mousePos;
}

glm::vec2 Input::Mouse::getMouseWheelScroll() const
{
    return m_mouseWheelScrollDelta;
}

void Input::Mouse::setCursorDisabled(bool locked) const
{
    if (m_disabledAction == nullptr)
    {
        HGError("Can't disable cursor, in case of unavailable action.");
        return;
    }

    m_disabledAction(locked);
}

void Input::Mouse::setCursorHidden(bool hidden) const
{
    if (m_hiddenAction == nullptr)
    {
        HGError("Can't hide cursor, in case of unavailable action.");
        return;
    }

    m_hiddenAction(hidden);
}

bool Input::Mouse::isCursorDisabled() const
{
    if (m_isDisabledAction == nullptr)
    {
        HGError("Can't check is cursor disabled, in case of unavailable action.");
        return false;
    }

    return m_isDisabledAction();
}

bool Input::Mouse::isCursorHidden() const
{
    if (m_isHiddenAction == nullptr)
    {
        HGError("Can't check is cursor hidden, in case of unavailable action.");
        return false;
    }

    return m_isHiddenAction();
}

void Input::Mouse::setCursorDisabledAction(std::function<void(bool)> disabledAction)
{
    m_disabledAction = std::move(disabledAction);
}

void Input::Mouse::setCursorHiddenAction(std::function<void(bool)> hiddenAction)
{
    m_hiddenAction = std::move(hiddenAction);
}

void Input::Mouse::setIsCursorDisabledAction(std::function<bool()> isDisabledAction)
{
    m_isDisabledAction = std::move(isDisabledAction);
}

void Input::Mouse::setIsCursorHiddenAction(std::function<bool()> isHiddenAction)
{
    m_isHiddenAction = std::move(isHiddenAction);
}

void Input::Mouse::setMousePosition(int x, int y)
{
    m_mousePos.x = x;
    m_mousePos.y = y;
}

void Input::Mouse::setMouseWheelScroll(float x, float y)
{
    m_mouseWheelScrollDelta.x = x;
    m_mouseWheelScrollDelta.y = y;
}

void Input::Mouse::setPressedButton(std::uint8_t button, bool pressed)
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

void Input::Mouse::tick()
{
    for (auto&& state : m_buttonStates)
    {
        state.second.justReleased = false;
        state.second.justPushed   = false;
    }

    m_mouseWheelScrollDelta.x = 0;
    m_mouseWheelScrollDelta.y = 0;
}

Input::Gamepads::Gamepads() : m_gamepads()
{
}

bool Input::Gamepads::isConnected() const
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

std::uint8_t Input::Gamepads::numberOfGamepadsConnected() const
{
    std::uint8_t numberOfGamepads = 0;

    for (const auto& [gamepad_index, gamepad_data] : m_gamepads)
    {
        if (gamepad_data.connected)
        {
            ++numberOfGamepads;
        }
    }

    return numberOfGamepads;
}

bool Input::Gamepads::isButtonPressed(std::uint8_t gamepad, Input::Gamepads::XBoxButtons button) const
{
    return isButtonPressed(gamepad, std::uint8_t(button));
}

bool Input::Gamepads::isButtonPressed(std::uint8_t gamepad, std::uint8_t button) const
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

bool Input::Gamepads::isButtonPushed(std::uint8_t gamepad, Input::Gamepads::XBoxButtons button) const
{
    return isButtonPushed(gamepad, std::uint8_t(button));
}

bool Input::Gamepads::isButtonPushed(std::uint8_t gamepad, std::uint8_t button) const
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

bool Input::Gamepads::isButtonReleased(std::uint8_t gamepad, Input::Gamepads::XBoxButtons button) const
{
    return isButtonReleased(gamepad, std::uint8_t(button));
}

bool Input::Gamepads::isButtonReleased(std::uint8_t gamepad, std::uint8_t button) const
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

std::uint8_t Input::Gamepads::numberOfButtons(std::uint8_t gamepad) const
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        return 0;
    }

    return gamepadIterator->second.numberOfButtons;
}

float Input::Gamepads::axisValue(std::uint8_t gamepad, Input::Gamepads::XBoxAxises axis) const
{
    return axisValue(gamepad, std::uint8_t(axis));
}

float Input::Gamepads::axisValue(std::uint8_t gamepad, std::uint8_t axisIndex) const
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

void Input::Gamepads::setIsConnectedGamepad(std::uint8_t gamepad, bool connected)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator != m_gamepads.end())
    {
        gamepadIterator->second.connected = connected;
    }
    else
    {
        m_gamepads[gamepad]           = GamepadData();
        m_gamepads[gamepad].connected = connected;
    }
}

void Input::Gamepads::setGamepadAxisValue(std::uint8_t gamepad, std::uint8_t axis, float value)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        m_gamepads[gamepad] = GamepadData();
        gamepadIterator     = m_gamepads.find(gamepad);
    }

    gamepadIterator->second.axises[axis] = value;
}

void Input::Gamepads::setGamepadButtonValue(std::uint8_t gamepad, std::uint8_t button, bool pressed)
{
    auto gamepadIterator = m_gamepads.find(gamepad);

    if (gamepadIterator == m_gamepads.end())
    {
        m_gamepads[gamepad] = GamepadData();
        gamepadIterator     = m_gamepads.find(gamepad);
    }

    if (pressed)
    {
        gamepadIterator->second.buttons[button].justPushed = true;
        gamepadIterator->second.buttons[button].pressed    = true;
    }
    else
    {
        gamepadIterator->second.buttons[button].justReleased = true;
    }
}

void Input::Gamepads::tick()
{
    for (auto& m_gamepad : m_gamepads)
    {
        for (auto& button : m_gamepad.second.buttons)
        {
            button.second.justPushed   = false;
            button.second.justReleased = false;
        }
    }
}

Input::Window::Window() : m_closed(false)
{
}

bool Input::Window::isClosed() const
{
    return m_closed;
}

void Input::Window::setClosed(bool closed)
{
    m_closed = closed;
}

Input::Input() : m_keyboard(), m_mouse(), m_gamepads(), m_window()
{
}

const Input::Keyboard* Input::keyboard() const
{
    return &m_keyboard;
}

const Input::Mouse* Input::mouse() const
{
    return &m_mouse;
}

const Input::Gamepads* Input::gamepads() const
{
    return &m_gamepads;
}

const Input::Window* Input::window() const
{
    return &m_window;
}

void Input::tickControllers()
{
    m_keyboard.tick();
    m_mouse.tick();
    m_gamepads.tick();
}
} // namespace HG::Core
