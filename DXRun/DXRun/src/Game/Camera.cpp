#include "Camera.h"
#include <cmath>

using namespace DirectX;

// ===== 内部カメラ情報 =====
struct SCameraInfo
{
    CVector3 Position;
    CVector3 LookAtPosition;
    CVector3 OrigPosition;
    CVector3 OrigLookAtPosition;
    CVector3 TargetPosition;
    CVector3 TargetLookAtPosition;

    float HAngle;
    float VAngle;

    CVector3 Direction;
    CVector3 RightDirection;
    CVector3 FrontDirection;

    bool  Shake;
    float ShakeWidth;
    float ShakeTime;
    float ShakeTimeCounter;
    float ShakeAngle;
    float ShakeAngleSpeed;
    CVector3 ShakePosition;
};

static SCameraInfo g_CamInfo;

// ---- 定数 ----
#define PLAYER_DISTANCE     (1000.0f)
#define PLAYER_OFFSET_Y     (120.0f)
#define DEFAULT_H_ANGLE     (DX_PI_F)
#define DEFAULT_V_ANGLE     (0.4f)
#define MOVE_DIV_NUM        (0.17f)

// ================= 初期化 =================
void Camera::initialize()
{
    g_CamInfo.Position = { 0.0f, 2500.f, -3500.f };
    g_CamInfo.HAngle = DEFAULT_H_ANGLE;
    g_CamInfo.VAngle = DEFAULT_V_ANGLE;
    g_CamInfo.Shake = false;
    g_CamInfo.ShakePosition = CVector3(0, 0, 0);
}

// ================= 更新 =================
void Camera::update(float dt)
{
    // ---- Shake ----
    if (g_CamInfo.Shake)
    {
        g_CamInfo.ShakePosition.setY(
            std::sin(g_CamInfo.ShakeAngle) *
            (1.0f - g_CamInfo.ShakeTimeCounter / g_CamInfo.ShakeTime) *
            g_CamInfo.ShakeWidth);

        g_CamInfo.ShakeAngle += g_CamInfo.ShakeAngleSpeed * dt;
        g_CamInfo.ShakeTimeCounter += dt;

        if (g_CamInfo.ShakeTimeCounter >= g_CamInfo.ShakeTime)
            g_CamInfo.Shake = false;
    }
    else
    {
        g_CamInfo.ShakePosition = CVector3(0, 0, 0);
    }

    if (auto target = _target.lock())
    {
        g_CamInfo.TargetLookAtPosition = target->getTargetPosition();
        g_CamInfo.TargetLookAtPosition.setY(
            g_CamInfo.TargetLookAtPosition.getY() + PLAYER_OFFSET_Y);

        // オービット計算
        CVector3 offset;
        offset.setZ(std::cos(g_CamInfo.VAngle) * PLAYER_DISTANCE);
        offset.setY(std::sin(g_CamInfo.VAngle) * PLAYER_DISTANCE);
        offset.setX(-std::sin(g_CamInfo.HAngle) * offset.getZ());
        offset.setZ(std::cos(g_CamInfo.HAngle) * offset.getZ());

        g_CamInfo.TargetPosition =
            g_CamInfo.TargetLookAtPosition + offset;

        float moveScale = dt / MOVE_DIV_NUM;

       /* g_CamInfo.OrigPosition +=
            (g_CamInfo.TargetPosition - g_CamInfo.OrigPosition) * moveScale;*/

        g_CamInfo.OrigLookAtPosition +=
            (g_CamInfo.TargetLookAtPosition - g_CamInfo.OrigLookAtPosition) * moveScale;

        /*g_CamInfo.Position =
            g_CamInfo.OrigPosition + g_CamInfo.ShakePosition;*/

        g_CamInfo.LookAtPosition =
            g_CamInfo.OrigLookAtPosition + g_CamInfo.ShakePosition;

        g_CamInfo.Direction =
            (g_CamInfo.LookAtPosition - g_CamInfo.Position).normalized();

        g_CamInfo.RightDirection =
            g_CamInfo.Direction.cross(CVector3(0, -1, 0)).normalized();

        g_CamInfo.FrontDirection =
            CVector3(0, -1, 0).cross(g_CamInfo.RightDirection).normalized();
    }

    applyToDxLib();
}

// ================= 行列構築 =================
void Camera::buildViewMatrix() const noexcept
{
    XMVECTOR eye = XMVectorSet(
        g_CamInfo.Position.getX(),
        g_CamInfo.Position.getY(),
        g_CamInfo.Position.getZ(), 1.0f);

    XMVECTOR at = XMVectorSet(
        g_CamInfo.LookAtPosition.getX(),
        g_CamInfo.LookAtPosition.getY(),
        g_CamInfo.LookAtPosition.getZ(), 1.0f);

    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    XMStoreFloat4x4(&_view, XMMatrixLookAtLH(eye, at, up));
}

void Camera::buildProjectionMatrix() const noexcept
{
    auto screenSize = Engine::getInstance().getScreenSize();

    XMStoreFloat4x4(
        &_proj,
        XMMatrixPerspectiveFovLH(
            CAMERA_FOV * XM_PI / 180.0f,
            screenSize.getX() / screenSize.getY(), // アスペクト比計算
            CAMERA_NEAR_Z,
            CAMERA_FAR_Z));
}

// ================= CameraBase =================
const XMFLOAT4X4& Camera::getViewMatrix() const noexcept
{
    buildViewMatrix();
    return _view;
}

const XMFLOAT4X4& Camera::getProjectionMatrix() const noexcept
{
    buildProjectionMatrix();
    return _proj;
}

const CVector3& Camera::getPosition() const noexcept
{
    return g_CamInfo.Position;
}

const CVector3& Camera::getForward() const noexcept
{
    return g_CamInfo.Direction;
}

const CVector3& Camera::getRight() const noexcept
{
    return g_CamInfo.RightDirection;
}

const CVector3& Camera::getUp() const noexcept
{
    static CVector3 up(0, 1, 0);
    return up;
}

// ================= DXLib 適用 =================
void Camera::applyToDxLib() const noexcept
{
    DxLib::SetCameraPositionAndTarget_UpVecY(
        DxLib::VGet(
            g_CamInfo.Position.getX(),
            g_CamInfo.Position.getY(),
            g_CamInfo.Position.getZ()),
        DxLib::VGet(
            g_CamInfo.LookAtPosition.getX(),
            g_CamInfo.LookAtPosition.getY(),
            g_CamInfo.LookAtPosition.getZ()));

    DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(
        DxLib::VGet(
            g_CamInfo.Position.getX(),
            g_CamInfo.Position.getY(),
            g_CamInfo.Position.getZ()),
        DxLib::VGet(
            g_CamInfo.LookAtPosition.getX(),
            g_CamInfo.LookAtPosition.getY(),
            g_CamInfo.LookAtPosition.getZ()));
}

// ================= 揺れ =================
void Camera::shake(float w, float t, float s)
{
    g_CamInfo.Shake = true;
    g_CamInfo.ShakeWidth = w;
    g_CamInfo.ShakeTime = t;
    g_CamInfo.ShakeTimeCounter = 0;
    g_CamInfo.ShakeAngle = 0;
    g_CamInfo.ShakeAngleSpeed = s;
}
