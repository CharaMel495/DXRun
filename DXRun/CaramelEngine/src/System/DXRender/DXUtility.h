#pragma once
#include "DXInclude.h"

inline D2D1_COLOR_F MakeColor(float r, float g, float b, float a = 1.0f) 
{
    return D2D1::ColorF(r, g, b, a);
}

inline D2D1_RECT_F MakeRect(float x, float y, float w, float h) 
{
    return D2D1::RectF(x, y, x + w, y + h);
}