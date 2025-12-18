#pragma once
#include "CaramelEngine.h"

using namespace CaramelEngine;

/// <summary>
/// 自分の座標を教えるインターフェース
/// </summary>
class ITargetProvider
{
public:

    inline ITargetProvider() noexcept
    {}

    virtual ~ITargetProvider() noexcept {}

    virtual CVector3 getTargetPosition() const noexcept = 0;

private:
};