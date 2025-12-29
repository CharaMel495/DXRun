#include "DxLibLight.h"

DxLibLight::DxLibLight()
{
    // DxLibのライト機能を有効化
    SetUseLighting(TRUE);

    // デフォルト環境光（暗すぎると何も見えないので）
    SetGlobalAmbientLight({ 0.2f, 0.2f, 0.2f, 1.0f });
}

DxLibLight::~DxLibLight()
{
    // 明示的に無効化
    SetLightEnable(FALSE);
}

void DxLibLight::Apply(const LightDesc& desc)
{
    if (!desc.enable)
    {
        Disable();
        return;
    }

    SetLightEnable(TRUE);
    SetGlobalAmbientLight(desc.ambient);

    switch (desc.type)
    {
    case LightType::Directional:
        ApplyDirectional(desc);
        break;

    case LightType::Point:
        ApplyPoint(desc);
        break;

    case LightType::Spot:
        // DxLib標準ではほぼ非対応なので今は何もしない
        break;
    }
}

void DxLibLight::Disable()
{
    SetLightEnable(FALSE);
}

void DxLibLight::ApplyDirectional(const LightDesc& desc)
{
    // 方向は正規化しておく
    VECTOR dir = VNorm(desc.direction);

    SetLightDirection(dir);
    SetLightDifColor(desc.diffuse);
}

void DxLibLight::ApplyPoint(const LightDesc& desc)
{
    SetLightPosition(desc.position);

    // 減衰・範囲の設定もできる
    SetLightRangeAtten(desc.range, 1.0f, 0.0f, 0.0f);

    SetLightDifColor(desc.diffuse);
}
