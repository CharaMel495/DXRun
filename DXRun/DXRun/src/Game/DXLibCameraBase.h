#pragma once
#include "CameraBase.h"

class DXLibCameraBase : public CaramelEngine::CameraBase
{
public:
    virtual ~DXLibCameraBase() = default;

    // DXLib ‚ÉƒJƒƒ‰ó‘Ô‚ğ”½‰f
    virtual void applyToDxLib() const noexcept = 0;
};