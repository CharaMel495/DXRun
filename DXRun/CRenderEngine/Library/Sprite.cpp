#include "pch.h"
#include "Sprite.h"

namespace CaramelEngine
{
    Sprite::Sprite(const std::string& name,
        ID3D11ShaderResourceView* srv,
        const CUVRect& uvRect,
        const CVector2& pivot) noexcept
        : _name(name),
        _srv(srv),
        _uvRect(uvRect),
        _pivot(pivot)
    {
    }
}
