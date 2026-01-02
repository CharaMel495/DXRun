#pragma once

#include <DirectXMath.h>
#include "CVector3.h"

namespace CaramelEngine
{
    class CameraBase
    {
    public:
        virtual ~CameraBase() = default;

        virtual void initialize() = 0;
        virtual void update(float deltaTime) = 0;

        // === çsóÒ ===
        virtual const DirectX::XMFLOAT4X4& getViewMatrix() const noexcept = 0;
        virtual const DirectX::XMFLOAT4X4& getProjectionMatrix() const noexcept = 0;

        // === ãÛä‘èÓïÒ ===
        virtual const CVector3& getPosition() const noexcept = 0;
        virtual const CVector3& getForward()  const noexcept = 0;
        virtual const CVector3& getRight()    const noexcept = 0;
        virtual const CVector3& getUp()       const noexcept = 0;
    };
}