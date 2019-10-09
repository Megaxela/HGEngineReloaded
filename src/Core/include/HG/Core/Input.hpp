#pragma once

// C++ STL
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

// GLM
#include <glm/vec2.hpp>

namespace HG::Core
{
/**
 * @brief Class, that aggregates
 * keyboard, mouse and gamepad input.
 */
class Input
{
public:
    /**
     * @brief Internal class, that describes
     * keyboard input.
     */
    class Keyboard
    {
    public:
        /**
         * @brief Key pressed modifiers.
         */
        enum class Modifiers
        {
            None //< Null
            ,
            First //< First value for iteration
            ,
            Shift = First //< Shift button.
            ,
            Ctrl //< Ctrl button.
            ,
            Alt //< Alt button.
            ,
            Last = Alt //< Last value for iteration
        };

        /**
         * @brief Keyboard keys enum.
         */
        enum class Key
        {
            None //< Null key
            ,
            First //< First value for iteration
            ,
            ESC = First //< Keyboard Escape button
            ,
            F1 //< Keyboard F1 button
            ,
            F2 //< Keyboard F2 button
            ,
            F3 //< Keyboard F3 button
            ,
            F4 //< Keyboard F4 button
            ,
            F5 //< Keyboard F5 button
            ,
            F6 //< Keyboard F6 button
            ,
            F7 //< Keyboard F7 button
            ,
            F8 //< Keyboard F8 button
            ,
            F9 //< Keyboard F9 button
            ,
            F10 //< Keyboard F10 button
            ,
            F11 //< Keyboard F11 button
            ,
            F12 //< Keyboard F12 button
            ,
            F13 //< Keyboard F13 button
            ,
            F14 //< Keyboard F14 button
            ,
            F15 //< Keyboard F15 button
            ,
            F16 //< Keyboard F16 button
            ,
            F17 //< Keyboard F17 button
            ,
            F18 //< Keyboard F18 button
            ,
            F19 //< Keyboard F19 button
            ,
            F20 //< Keyboard F10 button
            ,
            F21 //< Keyboard F21 button
            ,
            F22 //< Keyboard F22 button
            ,
            F23 //< Keyboard F23 button
            ,
            F24 //< Keyboard F24 button
            ,
            F25 //< Keyboard F25 button
            ,
            Tilda //< Keyboard tilda (~) button
            ,
            N1 //< Keyboard number 1 button
            ,
            N2 //< Keyboard number 2 button
            ,
            N3 //< Keyboard number 3 button
            ,
            N4 //< Keyboard number 4 button
            ,
            N5 //< Keyboard number 5 button
            ,
            N6 //< Keyboard number 6 button
            ,
            N7 //< Keyboard number 7 button
            ,
            N8 //< Keyboard number 8 button
            ,
            N9 //< Keyboard number 9 button
            ,
            N0 //< Keyboard number 0 button
            ,
            Minus //< Keyboard minus (-) button
            ,
            Equal //< Keyboard equal sign (=) button
            ,
            Backspace //< Keyboard backspace button
            ,
            Tab //< Keyboard tab button
            ,
            Q //< Keyboard letter Q button
            ,
            W //< Keyboard letter W button
            ,
            E //< Keyboard letter E button
            ,
            R //< Keyboard letter R button
            ,
            T //< Keyboard letter T button
            ,
            Y //< Keyboard letter Y button
            ,
            U //< Keyboard letter U button
            ,
            I //< Keyboard letter I button
            ,
            O //< Keyboard letter O button
            ,
            P //< Keyboard letter P button
            ,
            SquareBracketOpen //< Keyboard open square bracket ([) button
            ,
            SquareBracketClose //< Keyboard close square bracket (]) button
            ,
            Backslash //< Keyboard left slash (backslash) button (\)
            ,
            CapsLock //< Keyboard CapsLock button.
            ,
            A //< Keyboard letter A button
            ,
            S //< Keyboard letter S button
            ,
            D //< Keyboard letter D button
            ,
            F //< Keyboard letter F button
            ,
            G //< Keyboard letter G button
            ,
            H //< Keyboard letter H button
            ,
            J //< Keyboard letter J button
            ,
            K //< Keyboard letter K button
            ,
            L //< Keyboard letter L button
            ,
            Semicolon //< Keyboard semicolon (;) button
            ,
            SingleQuotation //< Keyboard single quotation mark (')
            ,
            Return //< Keyboard return (enter) button
            ,
            LeftShift //< Keyboard left shift button
            ,
            Z //< Keyboard letter Z button
            ,
            X //< Keyboard letter X button
            ,
            C //< Keyboard letter C button
            ,
            V //< Keyboard letter V button
            ,
            B //< Keyboard letter B button
            ,
            N //< Keyboard letter N button
            ,
            M //< Keyboard letter M button
            ,
            Comma //< Keyboard comma sign (,) button
            ,
            Dot //< Keyboard dot sign (.) button
            ,
            Slash //< Keyboard slash (/) button
            ,
            RightShift //< Keyboard right shift button
            ,
            LeftCtrl //< Keyboard left ctrl button
            ,
            RightSuper //< Keyboard right super (Win) button
            ,
            LeftSuper //< Keyboard left super (Win) button
            ,
            LeftAlt //< Keyboard left alt button
            ,
            Space //< Keyboard space button
            ,
            RightAlt //< Keyboard right alt button
            ,
            RightCtrl //< Keyboard right ctrl button
            ,
            ArrowLeft //< Keyboard arrow to the left
            ,
            ArrowUp //< Keyboard arrow to the up
            ,
            ArrowDown //< Keyboard arrow to the down
            ,
            ArrowRight //< Keyboard arrow to the right
            ,
            Insert //< Keyboard insert button
            ,
            Delete //< Keyboard delete button
            ,
            PageUp //< Keyboard page up button
            ,
            PageDown //< Keyboard page down button
            ,
            Home //< Keyboard home button
            ,
            End //< Keyboard end button
            ,
            NumLock //< Keyboard num lock button
            ,
            PrintScreen //< Keyboard print screen button
            ,
            Pause //< Keyboard pause button
            ,
            Menu //< Menu button
            ,
            Num0 //< Key pad 0 button
            ,
            Num1 //< Key pad 1 button
            ,
            Num2 //< Key pad 2 button
            ,
            Num3 //< Key pad 3 button
            ,
            Num4 //< Key pad 4 button
            ,
            Num5 //< Key pad 5 button
            ,
            Num6 //< Key pad 6 button
            ,
            Num7 //< Key pad 7 button
            ,
            Num8 //< Key pad 8 button
            ,
            Num9 //< Key pad 9 button
            ,
            NumDot //< Key pad dot button
            ,
            NumDivide //< Key pad divide button
            ,
            NumMultiply //< Key pad multiply button
            ,
            NumSubstract //< Key pad substract button
            ,
            NumAdd //< Key pad add button
            ,
            NumReturn //< Key pad return (enter) button
            ,
            NumEqual //< Key pad equal button
            ,
            Last = NumEqual //< System value. Used for detecting number of elements in enum
        };

        /**
         * @brief Constructor.
         */
        Keyboard();

        /**
         * @brief Method to get is keyboard key is pressed.
         * @param key Key enum value.
         * @return Pressed state.
         */
        bool isPressed(Key key) const;

        /**
         * @brief Method to get is keyboard key was just pushed.
         * @param key Key enum value.
         * @return Is just pushed.
         */
        bool isPushed(Key key) const;

        /**
         * @brief Method to get is keyboard key was just released.
         * @param key Key enum value.
         * @return
         */
        bool isReleased(Key key) const;

        /**
         * @brief Method to get is keyboard mofifier is pressed.
         * @param modifier Modifier value.
         * @return Pressed state.
         */
        bool isModifierPressed(Modifiers modifier) const;

        /**
         * Method to get pressed character. If there was no
         * pressed character function will return 0.
         * @return Character codepoint or 0.
         */
        uint32_t pressedCharacter() const;

        // Control method from now

        /**
         * @brief Method for settings keyboard key.
         * @param key Key.
         * @param pressed Pressed state.
         */
        void setPressed(Key key, bool pressed);

        /**
         * @brief Method for setting modifiers.
         * @param modifiers Modifier.
         * @param pressed Pressed state.
         */
        void setPressed(Modifiers modifiers, bool pressed);

        /**
         * @bried Method for setting entered character.
         * @param codepoint Character code point.
         */
        void setCharacterEntered(uint32_t codepoint);

        /**
        * @brief Method for notifying input controller
        * about frame changed to clean released and pushed states.
        */
        void tick();

    private:
        std::vector<bool> m_pushed;
        std::vector<bool> m_released;
        std::vector<bool> m_pressed;
        uint8_t m_pressedModifiers; // Bitfield
        uint32_t m_pressedCharacter;
    };

    /**
     * @brief Internal class, that describes
     * mouse input.
     */
    class Mouse
    {
    public:
        /**
         * @brief Enum with main mouse buttons.
         */
        enum class Buttons
        {
            Left = 0 //< Left mouse button
            ,
            Right = 1 //< Right mouse button
            ,
            Middle = 2 //< Middle mouse button
            ,
            None //< Null button
        };

        /**
         * @brief Constructor.
         */
        Mouse();

        /**
         * @brief Method for checking is mouse button pressed.
         * @param button Button enum.
         * @return Is pressed.
         */
        bool isPressed(Buttons button) const;

        /**
         * @brief Method for checking is mouse button pressed.
         * @param button Button number.
         * @return Is pressed.
         */
        bool isPressed(uint8_t button) const;

        /**
         * @brief Method for checking is mouse button just pushed.
         * @param button Button enum.
         * @return Is just pushed.
         */
        bool isPushed(Buttons button) const;

        /**
         * @brief Method for checking is mouse button just pushed.
         * @param button Button number.
         * @return Is just pushed.
         */
        bool isPushed(uint8_t button) const;

        /**
         * @brief Method for checking is mouse button just released.
         * @param button Button enum.
         * @return Is just released.
         */
        bool isReleased(Buttons button) const;

        /**
         * @brief Method for checking is mouse button just released.
         * @param button Button number.
         * @return Is just released.
         */
        bool isReleased(uint8_t button) const;

        /**
         * @brief Method for taking mouse position. Locally to window.
         */
        glm::vec2 getMousePosition() const;

        /**
         * @brief Method for taking mouse wheel scroll delta.
         */
        glm::vec2 getMouseWheelScroll() const;

        /**
         * @brief Method for locking mouse cursor.
         * @param locked Is cursor has to be locked.
         */
        void setCursorDisabled(bool locked) const;

        /**
         * @brief Method for hiding mouse cursor.
         * @param hidden Is cursor has to be hidden.
         */
        void setCursorHidden(bool hidden) const;

        // Control methods from now

        /**
         * @brief Method for setting cursor disabling action.
         * @param disabledAction Function with actual setting.
         */
        void setCursorDisabledAction(std::function<void(bool)> disabledAction);

        /**
         * @brief Method for setting cursor hiding action.
         * @param hiddenAction Function with actual setting.
         */
        void setCursorHiddenAction(std::function<void(bool)> hiddenAction);

        /**
         * @brief Method for setting mouse position. Locally to window.
         * @param x X position.
         * @param y Y position.
         */
        void setMousePosition(int x, int y);

        /**
         * @brief Method for getting mouse wheel scroll delta.
         * @param x X delta.
         * @param y Y delta.
         */
        void setMouseWheelScroll(float x, float y);

        /**
         * @brief Method for setting mouse button pressed state.
         * @param button Button number.
         * @param pressed Pressed state.
         */
        void setPressedButton(uint8_t button, bool pressed);

        /**
        * @brief Method for notifying input controller
        * about frame changed to clean released and pushed states.
        */
        void tick();

    private:
        struct ButtonState
        {
            bool justPushed : 1;
            bool justReleased : 1;
            bool pressed : 1;
        };

        glm::vec2 m_mousePos;
        glm::vec2 m_mouseWheelScrollDelta;
        std::unordered_map<uint8_t, ButtonState> m_buttonStates;

        std::function<void(bool)> m_disabledAction;
        std::function<void(bool)> m_hiddenAction;
    };

    /*
     * @brief Internal class, that describes
     * gamepad input.
     */
    class Gamepads
    {
    public:
        /**
         * @brief XBox gamepad buttons enums.
         */
        enum class XBoxButtons
        {
            X                   = 2,
            Y                   = 3,
            B                   = 1,
            A                   = 0,
            Left                = 13,
            Right               = 11,
            Top                 = 10,
            Down                = 12,
            RightBumper         = 5,
            LeftBumper          = 4,
            Start               = 6,
            Select              = 7,
            LeftJoystickButton  = 8,
            RightJoystickButton = 9
        };

        /**
         * @brief XBox gamepad axises enums.
         */
        enum class XBoxAxises
        {
            LeftAxisX    = 0,
            LeftAxisY    = 1,
            RightAxisX   = 2,
            RightAxisY   = 3,
            LeftTrigger  = 4,
            RightTrigger = 5
        };

        /**
         * @brief Controller.
         */
        Gamepads();

        /**
         * @brief Method for checking is any
         * gamepad is connected to host.
         * @return Is any gamepad connected.
         */
        bool isConnected() const;

        /**
         * @brief Method for getting number of connected
         * gamepads.
         * @return Number of gamepads.
         */
        uint8_t numberOfGamepadsConnected() const;

        /**
         * @brief Method for getting is one of XBox
         * gamepads button pressed. (It works with
         * any gamepad, but with XBox button index mapping)
         * @param gamepad Gamepad index.
         * @param button XBoxButtons enum value.
         * @return Is button in pressed state.
         */
        bool isButtonPressed(uint8_t gamepad, XBoxButtons button) const;

        /**
         * @brief Method for getting is one of
         * gamepads button pressed.
         * @param gamepad Gamepad index.
         * @param button Button number.
         * @return Is button in pressed state.
         */
        bool isButtonPressed(uint8_t gamepad, uint8_t button) const;

        /**
         * @brief Method to get is gamepad button pushed.
         * @param gamepad Gamepad index.
         * @param button XBoxButtons enum value.
         * @return Button pushed state.
         */
        bool isButtonPushed(uint8_t gamepad, XBoxButtons button) const;

        /**
         * @brief Method to get is gamepad button pushed.
         * @param gamepad Gamepad index.
         * @param button Gamepad button index.
         * @return Button pushed state.
         */
        bool isButtonPushed(uint8_t gamepad, uint8_t button) const;

        /**
         * @brief Method to get is gamepad button released.
         * @param gamepad Gamepad index.
         * @param button XBoxButtons enum value.
         * @return Button released state.
         */
        bool isButtonReleased(uint8_t gamepad, XBoxButtons button) const;

        /**
         * @brief Method to get is gamepad button released.
         * @param gamepad Gamepad index.
         * @param button Gamepad button index.
         * @return Button released state.
         */
        bool isButtonReleased(uint8_t gamepad, uint8_t button) const;

        /**
         * @brief Method to get number of buttons in gamepad.
         * @param gamepad Gamepad index.
         * @return Number of buttons.
         */
        uint8_t numberOfButtons(uint8_t gamepad) const;

        /**
         * @brief Method to get gamepad axis value.
         * @param gamepad Gamepad index.
         * @param axis XBoxAxises enum value.
         * @return Vector2 axis value.
         */
        float axisValue(uint8_t gamepad, XBoxAxises axis) const;

        /**
         * @brief Method to get gamepad axis value.
         * @param gamepad Gamepad index.
         * @param axisIndex Axis index.
         * @return Vector2 axis value.
         */
        float axisValue(uint8_t gamepad, uint8_t axisIndex) const;

        // Control method from now

        /**
         * @brief Method for setting gamepad connected state.
         * @param gamepad Gamepad index.
         * @param connected Connected state.
         */
        void setIsConnectedGamepad(uint8_t gamepad, bool connected);

        /**
         * @brief Method for setting gamepad axis value.
         * @param gamepad Gamepad index.
         * @param axis Gamepad axis index.
         * @param value Axis value.
         */
        void setGamepadAxisValue(uint8_t gamepad, uint8_t axis, float value);

        /**
         * @brief Method for setting gamepad button value.
         * @param gamepad Gamepad index.
         * @param button Button index.
         * @param pressed Button value.
         */
        void setGamepadButtonValue(uint8_t gamepad, uint8_t button, bool pressed);

        /**
        * @brief Method for notifying input controller
        * about frame changed to clean released and pushed states.
        */
        void tick();

    private:
        struct ButtonState
        {
            ButtonState() : justPushed(false), justReleased(false), pressed(false)
            {
            }

            bool justPushed : 1;
            bool justReleased : 1;
            bool pressed : 1;
        };

        struct GamepadData
        {
            GamepadData() :
                connected(false),
                buttons(),
                axises(),
                numberOfButtons(0),
                numberOfAxises(0),
                numberOfAnalogs(0)
            {
            }

            bool connected;
            std::unordered_map<uint8_t, ButtonState> buttons;
            std::unordered_map<uint8_t, float> axises;
            uint8_t numberOfButtons;
            uint8_t numberOfAxises;
            uint8_t numberOfAnalogs;
        };

        std::unordered_map<uint8_t, GamepadData> m_gamepads;
    };

    /**
     * @brief Method for checking any of
     * inputs on window.
     */
    class Window
    {
    public:
        /**
         * @brief Constructor.
         */
        Window();

        /**
         * @brief Method for checking is window closed.
         * @return Is window closed requested.
         */
        bool isClosed() const;

        // Control method from now

        /**
         * @brief Method for setting is window
         * closed.
         * @param closed Closed state.
         */
        void setClosed(bool closed);

    private:
        bool m_closed;
    };

    /**
     * @brief Constructor.
     */
    Input();

    /**
     * @brief Method for getting pointer to
     * keyboard.
     * @return Pointer to keyboard object.
     */
    const Keyboard* keyboard() const;

    /**
     * @brief Method for getting pointer to
     * mouse.
     * @return Pointer to mouse object.
     */
    const Mouse* mouse() const;

    /**
     * @brief Method for getting pointer to
     * gamepads info.
     * @return Pointer to gamepads object.
     */
    const Gamepads* gamepads() const;

    /**
     * @brief Method for getting pointer
     * to window info.
     * @return Pointer to window input info.
     */
    const Window* window() const;

    /**
     * @brief Method for notifying input controllers
     * about frame changed to clean released and pushed states.
     */
    void tickControllers();

private:
    Keyboard m_keyboard;
    Mouse m_mouse;
    Gamepads m_gamepads;

    Window m_window;
};
} // namespace HG::Core
