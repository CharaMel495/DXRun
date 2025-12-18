#pragma once
#include "InputKey.h"
#include "GamePadState.h"

namespace CaramelEngine
{
    /// <summary>
    /// パッドのアナログスティックの入力を受け取る用の構造体
    /// </summary>
    struct StickInput
    {
        float x;
        float y;
    };

    /// <summary>
    /// 入力を管理するクラス
    /// </summary>
    class InputManager
    {
    public:
        static InputManager& instance();

        void initialize();
        void update();
        void shutdown();
        void keyPressed(int rawKey);
        void keyReleased(int rawKey);

        bool isKeyPressed(int virtualKey);

        bool isDown(InputKey key) const;
        bool isHeld(InputKey key) const;
        bool isUp(InputKey key) const;

        bool isStickActiveR() const;
        bool isStickActiveL() const;
        bool isStickReleaseR() const;
        bool isStickReleaseL() const;

        bool isAnyKeyDown() const;
        bool isAnyKeyHeld() const;
        bool isAnyKeyUp() const;

        Cvector<InputKey> getPressedKeys() const;
        Cvector<InputKey> getHeldKeys() const;
        Cvector<InputKey> getReleasedKeys() const;

        //左右スティックの入力を受け取る関数
        StickInput getLeftStick() const { return _leftStick; }
        StickInput getRightStick() const { return _rightStick; }

        InputManager() = default;
        ~InputManager() = default;
    private:

        std::unordered_map<InputKey, bool> _currentKeys;
        std::unordered_map<InputKey, bool> _previousKeys;

        GamepadState _pad;

        StickInput _leftStick;
        StickInput _rightStick;

        bool _isLeftStickActive;
        bool _isRightStickActive;
        bool _isLeftStickActivePrev;
        bool _isRightStickActivePrev;

        InputKey translateKey(int rawKey) const;
    };
}