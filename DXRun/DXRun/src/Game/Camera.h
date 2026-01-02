#pragma once

#include "DxLib.h"
#include <DirectXMath.h>
#include <memory>

#include "DXLibCameraBase.h"
#include "ITargetProvider.h"
#include "CaramelEngine.h"

// ---- íËêî ----
#define CAMERA_NEAR_Z   (20.0f)
#define CAMERA_FAR_Z    (25000.0f)
#define CAMERA_FOV      (40.0f)

// ---- Camera ----
class Camera final : public DXLibCameraBase
{
public:
    Camera() noexcept = default;
    ~Camera() override = default;

    void initialize() override;
    void update(float deltaTime) override;

    void shake(float width, float time, float angleSpeed);

    void setTarget(std::weak_ptr<ITargetProvider> target) noexcept
    {
        _target = target;
    }

    // === CameraBase é¿ëï ===
    const DirectX::XMFLOAT4X4& getViewMatrix() const noexcept override;
    const DirectX::XMFLOAT4X4& getProjectionMatrix() const noexcept override;

    const CVector3& getPosition() const noexcept override;
    const CVector3& getForward()  const noexcept override;
    const CVector3& getRight()    const noexcept override;
    const CVector3& getUp()       const noexcept override;

    // === DXCameraBase ===
    void applyToDxLib() const noexcept override;

private:
    void buildViewMatrix() const noexcept;
    void buildProjectionMatrix() const noexcept;

private:
    std::weak_ptr<ITargetProvider> _target;

    mutable DirectX::XMFLOAT4X4 _view;
    mutable DirectX::XMFLOAT4X4 _proj;
};
