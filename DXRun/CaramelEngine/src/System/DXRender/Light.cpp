#include "pch.h"
#include "Light.h"

using namespace DirectX;

namespace CaramelEngine
{
    //--------------------------------------------------------------------------------------
    Light::Light() :
        _directional{ 0.0f, -1.0f, 0.0f, 0.0f },
        _position{ 0.0f, 10.0f, 0.0f, 0.0f },
        _ambient{ 0.1f, 0.1f, 0.1f, 1.0f },
        _diffuse{ 1.0f, 1.0f, 1.0f, 1.0f },
        _speqular{ 1.0f, 1.0f, 1.0f, 1.0f },
        _attenuation{ 0.0f, 0.0f, 1.0f, 0.0f }
    {
    }

    //--------------------------------------------------------------------------------------
    void Light::SetDirectionalLight(const XMVECTOR directional)
    {
        _directional = XMVector3Normalize(directional);
    }

    //--------------------------------------------------------------------------------------
    void Light::SetLightPosition(const XMVECTOR position)
    {
        _position = position;
    }

    //--------------------------------------------------------------------------------------
    void Light::SetAmbientLight(const XMFLOAT4 ambient)
    {
        _ambient = ambient;
    }

    //--------------------------------------------------------------------------------------
    void Light::SetDiffuseLight(const XMFLOAT4 diffuse)
    {
        _diffuse = diffuse;
    }

    //--------------------------------------------------------------------------------------
    void Light::SetSpeqularLight(const XMFLOAT4 speqular)
    {
        _speqular = speqular;
    }

    //--------------------------------------------------------------------------------------
    // Attenuation = 1/( att0 + att1 * d + att2 * d^2)
    // att0 àÍíËÇÃå∏êäåWêî(attenuation.x)
    // att1 ê¸å`å∏êäåWêî(attenuation.y)
    // att2 ìÒéüå∏êäåWêî(attenuation.z)
    // d = | Ldir |
    void Light::SetAttenuation(const XMFLOAT4 attenuation)
    {
        _attenuation = attenuation;
    }
}