#pragma once

#include "CVector3.h"
#include "CVector2.h"

namespace CaramelEngine
{
    /// <summary>
    /// CVector3 Ç©ÇÁ CVector2 Ç÷ÇÃïœä∑
    /// </summary>
    inline CVector2 ToVector2D(const CVector3& vec3)
    {
        return CVector2(vec3.getX(), vec3.getY());
    }

    /// <summary>
    /// CVector2 Ç©ÇÁ CVector3 Ç÷ÇÃïœä∑Åiz = 0Åj
    /// </summary>
    inline CVector3 ToVector3D(const CVector2& vec2)
    {
        return CVector3(vec2.getX(), vec2.getY(), 0.0f);
    }
}
