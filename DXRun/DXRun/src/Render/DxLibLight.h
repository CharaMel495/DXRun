#pragma once
#include "DxLib.h"

using namespace DxLib;

// ライト種別
enum class LightType
{
    Directional,
    Point,
    Spot, // 将来用（DxLibでは未対応寄り）
};

// ライトの共通データ
struct LightDesc
{
    LightType type = LightType::Directional;

    VECTOR position = VGet(0.0f, 0.0f, 0.0f);
    VECTOR direction = VGet(0.0f, -1.0f, 0.0f);

    COLOR_F diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    COLOR_F ambient = { 0.0f, 0.0f, 0.0f, 1.0f };

    float range = 1000.0f;
    bool enable = true;
};

// DxLibライトの薄いラッパ
class DxLibLight
{
public:
    DxLibLight();
    ~DxLibLight();

    void Apply(const LightDesc& desc);
    void Disable();

private:
    void ApplyDirectional(const LightDesc& desc);
    void ApplyPoint(const LightDesc& desc);
};
