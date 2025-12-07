#pragma once

namespace CaramelEngine
{
    /// <summary>
    /// キー入力
    /// </summary>
    enum class InputKey
    {
        Unknown,

        // --- Keyboard ---
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Up, Down, Left, Right,
        Space, Enter, Escape,

        // --- Gamepad ---
        ButtonA, ButtonB, ButtonX, ButtonY,
        Start, Back,
        LShoulder, RShoulder,
        LThumb, RThumb,
        DPadUp, DPadDown, DPadLeft, DPadRight,

        LStickLeft, LStickRight, LStickUp, LStickDown,
        // トリガーはアナログとして別で管理
    };
}