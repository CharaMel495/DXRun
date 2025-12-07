#include "pch.h"
#include "XInput.h"

namespace CaramelEngine
{
    void GamepadState::update() 
    {
        _previous = _current;
        ZeroMemory(&_current, sizeof(XINPUT_STATE));
        XInputGetState(0, &_current);
    }

    bool GamepadState::isHeld(InputKey key) const 
    {
        WORD buttons = _current.Gamepad.wButtons;
        switch (key) {
        case InputKey::ButtonA: return buttons & XINPUT_GAMEPAD_A;
        case InputKey::ButtonB: return buttons & XINPUT_GAMEPAD_B;
        case InputKey::ButtonX: return buttons & XINPUT_GAMEPAD_X;
        case InputKey::ButtonY: return buttons & XINPUT_GAMEPAD_Y;
        case InputKey::Start:   return buttons & XINPUT_GAMEPAD_START;
        case InputKey::Back:    return buttons & XINPUT_GAMEPAD_BACK;
        case InputKey::LShoulder: return buttons & XINPUT_GAMEPAD_LEFT_SHOULDER;
        case InputKey::RShoulder: return buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        case InputKey::DPadUp:    return buttons & XINPUT_GAMEPAD_DPAD_UP;
        case InputKey::DPadDown:  return buttons & XINPUT_GAMEPAD_DPAD_DOWN;
        case InputKey::DPadLeft:  return buttons & XINPUT_GAMEPAD_DPAD_LEFT;
        case InputKey::DPadRight: return buttons & XINPUT_GAMEPAD_DPAD_RIGHT;
        default: return false;
        }
    }

    bool GamepadState::isDown(InputKey key) const 
    {
        WORD now = _current.Gamepad.wButtons;
        WORD before = _previous.Gamepad.wButtons;

        auto bit = [&]() -> WORD {
            switch (key) {
            case InputKey::ButtonA: return XINPUT_GAMEPAD_A;
            case InputKey::ButtonB: return XINPUT_GAMEPAD_B;
            case InputKey::ButtonX: return XINPUT_GAMEPAD_X;
            case InputKey::ButtonY: return XINPUT_GAMEPAD_Y;
            case InputKey::Start:   return XINPUT_GAMEPAD_START;
            case InputKey::Back:    return XINPUT_GAMEPAD_BACK;
            case InputKey::LShoulder: return XINPUT_GAMEPAD_LEFT_SHOULDER;
            case InputKey::RShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;
            case InputKey::DPadUp:    return XINPUT_GAMEPAD_DPAD_UP;
            case InputKey::DPadDown:  return XINPUT_GAMEPAD_DPAD_DOWN;
            case InputKey::DPadLeft:  return XINPUT_GAMEPAD_DPAD_LEFT;
            case InputKey::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;
            default: return 0;
            }
            }();

        return !(before & bit) && (now & bit);
    }

    bool GamepadState::isUp(InputKey key) const 
    {
        WORD now = _current.Gamepad.wButtons;
        WORD before = _previous.Gamepad.wButtons;
        WORD bit = 0;

        switch (key) {
        case InputKey::ButtonA: bit = XINPUT_GAMEPAD_A; break;
        case InputKey::ButtonB: bit = XINPUT_GAMEPAD_B; break;
        case InputKey::ButtonX: bit = XINPUT_GAMEPAD_X; break;
        case InputKey::ButtonY: bit = XINPUT_GAMEPAD_Y; break;
        case InputKey::Start: bit = XINPUT_GAMEPAD_START; break;
        case InputKey::Back: bit = XINPUT_GAMEPAD_BACK; break;
        case InputKey::LShoulder: bit = XINPUT_GAMEPAD_LEFT_SHOULDER; break;
        case InputKey::RShoulder: bit = XINPUT_GAMEPAD_RIGHT_SHOULDER; break;
        case InputKey::DPadUp: bit = XINPUT_GAMEPAD_DPAD_UP; break;
        case InputKey::DPadDown: bit = XINPUT_GAMEPAD_DPAD_DOWN; break;
        case InputKey::DPadLeft: bit = XINPUT_GAMEPAD_DPAD_LEFT; break;
        case InputKey::DPadRight: bit = XINPUT_GAMEPAD_DPAD_RIGHT; break;
        default: break;
        }

        return (before & bit) && !(now & bit);
    }
}