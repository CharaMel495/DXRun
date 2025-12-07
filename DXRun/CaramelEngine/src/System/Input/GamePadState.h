#pragma once

#include <Windows.h>
#include <Xinput.h>

namespace CaramelEngine
{
    class GamepadState
    {
    public:
        void update();

        bool isHeld(InputKey key) const;
        bool isDown(InputKey key) const;
        bool isUp(InputKey key) const;

    private:
        XINPUT_STATE _current{};
        XINPUT_STATE _previous{};
    };
}