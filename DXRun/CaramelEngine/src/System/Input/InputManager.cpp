#include "pch.h"
#include "InputManager.h"
#include <Windows.h>

namespace CaramelEngine
{
    // デッドゾーンの定義
    constexpr float DEADZONE = 0.2f;

    // デッドゾーンの2乗を定義
    constexpr float DEADZONE_SQUARED = 0.04f; // (0.2)^2

    // 正規化関数
    float normalizeStickValue(SHORT value)
    {
        const float MAX_VALUE = 32767.0f;
        float normalized = static_cast<float>(value) / MAX_VALUE;

        // デッドゾーン処理
        if (std::abs(normalized) < DEADZONE)
            return 0.0f;

        return normalized;
    }

    InputManager& InputManager::instance() 
    {
        static InputManager instance;
        return instance;
    }

    void InputManager::initialize()
    {
        // キーが存在しない場合はfalseで初期化
        for (const auto& [key, state] : _currentKeys)
        {
            if (!_previousKeys.contains(key))
            {
                _previousKeys[key] = false;
            }
        }
    }

    void InputManager::update() 
    {
        _previousKeys = _currentKeys;
        _isLeftStickActivePrev = _isLeftStickActive;
        _isRightStickActivePrev = _isRightStickActive;

        // Gamepad更新
        _pad.update();

        // XInputでゲームパッドの状態を取得
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        //スティックの入力を受け取る
        if (XInputGetState(0, &state) == ERROR_SUCCESS) // コントローラーID 0を使用
        {
            // 左スティック
            _leftStick.x = normalizeStickValue(state.Gamepad.sThumbLX);
            _leftStick.y = normalizeStickValue(state.Gamepad.sThumbLY);

            _isLeftStickActive = isStickActiveL();

            // 右スティック
            _rightStick.x = normalizeStickValue(state.Gamepad.sThumbRX);
            _rightStick.y = normalizeStickValue(state.Gamepad.sThumbRY);

            _isRightStickActive = isStickActiveR();
        }
        else
        {
            // コントローラーが接続されていない場合はゼロにリセット
            _leftStick = { 0.0f, 0.0f };
            _rightStick = { 0.0f, 0.0f };

            _isLeftStickActive = false;
            _isRightStickActive = false;
        }

        // 全キーをチェック
        for (int key = 0x00; key < 0xFF; ++key)
        {
            InputKey translatedKey = translateKey(key);
            if (translatedKey != InputKey::Unknown)
            {
                bool isPressed = isKeyPressed(key);
                _currentKeys[translatedKey] = isPressed;
            }
        }

        // GamepadキーをInputKeyで更新
        for (auto key : 
            {
            InputKey::ButtonA, InputKey::ButtonB, InputKey::ButtonX, InputKey::ButtonY,
            InputKey::Start, InputKey::Back, InputKey::LShoulder, InputKey::RShoulder,
            InputKey::DPadUp, InputKey::DPadDown, InputKey::DPadLeft, InputKey::DPadRight
            }) 
        {
            _currentKeys[key] = _pad.isHeld(key);
        }
    }

    void InputManager::shutdown()
    {

    }

    bool InputManager::isKeyPressed(int virtualKey)
    {
        return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
    }

    void InputManager::keyPressed(int rawKey) 
    {
        _currentKeys[translateKey(rawKey)] = true;
    }

    void InputManager::keyReleased(int rawKey) 
    {
        _currentKeys[translateKey(rawKey)] = false;
    }

    bool InputManager::isHeld(InputKey key) const 
    {
        return _currentKeys.contains(key) && _currentKeys.at(key);
    }

    bool InputManager::isDown(InputKey key) const 
    {
        return (!_previousKeys.contains(key) || !_previousKeys.at(key)) && isHeld(key);
    }

    bool InputManager::isUp(InputKey key) const 
    {
        return _previousKeys.contains(key) && _previousKeys.at(key) && !isHeld(key);
    }

    //右スティックが倒されているかチェック
    bool InputManager::isStickActiveR() const
    {
        // x^2 + y^2 > DEADZONE^2 で判定
        return (_rightStick.x * _rightStick.x + _rightStick.y * _rightStick.y) > DEADZONE_SQUARED;
    }

    //左スティックが倒されているかチェック
    bool InputManager::isStickActiveL() const
    {
        // x^2 + y^2 > DEADZONE^2 で判定
        return (_leftStick.x * _leftStick.x + _leftStick.y * _leftStick.y) > DEADZONE_SQUARED;
    }

    //右スティックが倒されているかチェック
    bool InputManager::isStickReleaseR() const
    {
        //現在の入力がないかつ、プレビューと異なる
        return !_isRightStickActive && (_isRightStickActive != _isRightStickActivePrev);
    }

    //左スティックが倒されているかチェック
    bool InputManager::isStickReleaseL() const
    {
        //現在の入力がないかつ、プレビューと異なる
        return !_isLeftStickActive && (_isLeftStickActive != _isLeftStickActivePrev);
    }

    bool InputManager::isAnyKeyDown() const
    {
        for (const auto& [key, state] : _currentKeys)
        {
            if (state && !_previousKeys.at(key))
            {
                return true;
            }
        }
        return false;
    }

    bool InputManager::isAnyKeyHeld() const
    {
        for (const auto& [key, state] : _currentKeys)
        {
            if (state)
            {
                return true;
            }
        }
        return false;
    }

    bool InputManager::isAnyKeyUp() const
    {
        //前に押されてるものが無ければ離されてるものはないハズ
        if (_previousKeys.size() < 1)
            return false;

        for (const auto& [key, state] : _currentKeys)
        {
            if (!state && _previousKeys.at(key))
            {
                return true;
            }
        }
        return false;
    }

    Cvector<InputKey> InputManager::getPressedKeys() const
    {
        Cvector<InputKey> pressedKeys;
        for (const auto& [key, state] : _currentKeys)
        {
            if (state && !_previousKeys.at(key))
            {
                pressedKeys.push_back(key);
            }
        }
        return pressedKeys;
    }

    Cvector<InputKey> InputManager::getHeldKeys() const
    {
        Cvector<InputKey> heldKeys;
        for (const auto& [key, state] : _currentKeys)
        {
            if (state)
            {
                heldKeys.push_back(key);
            }
        }
        return heldKeys;
    }

    Cvector<InputKey> InputManager::getReleasedKeys() const
    {
        Cvector<InputKey> releasedKeys;
        for (const auto& [key, state] : _currentKeys)
        {
            if (!state && _previousKeys.at(key))
            {
                releasedKeys.push_back(key);
            }
        }
        return releasedKeys;
    }

    InputKey InputManager::translateKey(int rawKey) const 
    {
        switch (rawKey) 
        {
        // Keyboard keys (A-Z)
        case 'A': return InputKey::A;
        case 'B': return InputKey::B;
        case 'C': return InputKey::C;
        case 'D': return InputKey::D;
        case 'E': return InputKey::E;
        case 'F': return InputKey::F;
        case 'G': return InputKey::G;
        case 'H': return InputKey::H;
        case 'I': return InputKey::I;
        case 'J': return InputKey::J;
        case 'K': return InputKey::K;
        case 'L': return InputKey::L;
        case 'M': return InputKey::M;
        case 'N': return InputKey::N;
        case 'O': return InputKey::O;
        case 'P': return InputKey::P;
        case 'Q': return InputKey::Q;
        case 'R': return InputKey::R;
        case 'S': return InputKey::S;
        case 'T': return InputKey::T;
        case 'U': return InputKey::U;
        case 'V': return InputKey::V;
        case 'W': return InputKey::W;
        case 'X': return InputKey::X;
        case 'Y': return InputKey::Y;
        case 'Z': return InputKey::Z;

        // Special keys
        case VK_UP: return InputKey::Up;
        case VK_DOWN: return InputKey::Down;
        case VK_LEFT: return InputKey::Left;
        case VK_RIGHT: return InputKey::Right;
        case VK_SPACE: return InputKey::Space;
        case VK_RETURN: return InputKey::Enter;
        case VK_ESCAPE: return InputKey::Escape;

        default: return InputKey::Unknown;
        }
    }
}