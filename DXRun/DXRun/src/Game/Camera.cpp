#include "Camera.h"
#include <math.h>

// カメラの当たり判定サイズ
#define COLLISION_SIZE              (15.0f)

// カメラの角度変更速度
#define ANGLE_SPEED                 (3.0f)

// カメラとプレイヤーの距離
#define PLAYER_DISTANCE             (1000.0f)

// カメラが注視点とするプレイヤーの座標のＹ座標に足す値
#define PLAYER_OFFSET_Y             (120.0f)

// カメラの初期水平角度
#define DEFAULT_H_ANGLE             (DX_PI_F)

// カメラの初期垂直角度
#define DEFAULT_V_ANGLE             (0.4f)

// カメラの画角
#define CAMERA_FOV                  (40.0f)

// カメラの座標を目標座標に近づける計算で除算に使用する値
#define MOVE_DIV_NUM                (0.17f)

// カメラの情報
typedef struct _SCameraInfo
{
    // カメラの座標
    CVector3 Position;

    // カメラの注視点座標
    CVector3 LookAtPosition;

    // カメラ揺らし座標を無視したカメラの座標
    CVector3 OrigPosition;

    // カメラ揺らし座標を無視したカメラの注視点座標
    CVector3 OrigLookAtPosition;

    // カメラの目標座標
    CVector3 TargetPosition;

    // カメラの目標注視点座標
    CVector3 TargetLookAtPosition;

    // カメラの注視点に対する水平角度
    float  HAngle;

    // カメラの注視点に対する垂直角度
    float  VAngle;

    // カメラの座標からカメラの注視点座標へのベクトル
    CVector3 Direction;

    // カメラから見て右方向のベクトル
    CVector3 RightDirection;

    // カメラの正面方向のＹ成分を抜いたベクトル
    CVector3 FrontDirection;

    // カメラを揺らす処理を行っているかどうか
    bool   Shake;

    // カメラを揺らす幅
    float  ShakeWidth;

    // カメラを揺らす時間
    float  ShakeTime;

    // カメラを揺らす処理の経過時間計測用変数
    float  ShakeTimeCounter;

    // カメラを揺らす処理で使用する sin に渡す角度
    float  ShakeAngle;

    // カメラを揺らす処理で使用する sin に渡す角度の変化速度
    float  ShakeAngleSpeed;

    // カメラを揺らす処理で算出したカメラ座標に足す座標
    CVector3 ShakePosition;
} SCameraInfo;

static SCameraInfo g_CamInfo;

// カメラの処理を初期化する
void Camera::initialize(void)
{
    // 各パラメータに初期値をセットする
    g_CamInfo.HAngle = DEFAULT_H_ANGLE;
    g_CamInfo.VAngle = DEFAULT_V_ANGLE;
    g_CamInfo.Shake = false;
    g_CamInfo.ShakePosition = CVector3(0.0f, 0.0f, 0.0f);

    setup();
}

// カメラの状態推移処理を実行する
void Camera::update(float StepTime)
{
    CVector3    CameraStickInput;
    float       MoveScale;
    CVector3    TempV;
    float       NotHitDistance;
    float       HitDistance;
    float       TestDistance;
    CVector3    TestPosition;
    CVector3    Direction;

    // プレイヤーのキャラ情報構造体のアドレスを取得する
    //PCInfo = System_GetPlayerCharaInfo();

    // プレイヤーが注視点になるので、プレイヤーが存在しない場合は何もせず終了
    /*if (PCInfo == NULL)
    {
        return;
    }*/

    // カメラを揺らす処理を行うかどうかのフラグが立っていたらカメラを揺らす処理を行う
    if (g_CamInfo.Shake)
    {
        // sin を使用した揺らし座標の算出
        g_CamInfo.ShakePosition.setY(
            sin(g_CamInfo.ShakeAngle) * (1.0f - (g_CamInfo.ShakeTimeCounter /
                g_CamInfo.ShakeTime)) * g_CamInfo.ShakeWidth);
        g_CamInfo.ShakePosition.setX(0.0f);
        g_CamInfo.ShakePosition.setZ(0.0f);

        // 揺らし処理に使用する sin に渡す角度の変更処理
        g_CamInfo.ShakeAngle += g_CamInfo.ShakeAngleSpeed * StepTime;

        // 揺らす時間が経過したら揺らし処理を終了する
        g_CamInfo.ShakeTimeCounter += StepTime;
        if (g_CamInfo.ShakeTimeCounter >= g_CamInfo.ShakeTime)
        {
            g_CamInfo.Shake = false;
        }
    }
    else
    {
        // 揺らされていない場合は揺らし処理による加算座標を０にする
        g_CamInfo.ShakePosition = CVector3(0.0f, 0.0f, 0.0f);
    }

    if (auto target = _target.lock())
    {

        // 目標注視点座標の算出
        g_CamInfo.TargetLookAtPosition = target->getTargetPosition();
        g_CamInfo.TargetLookAtPosition.setY(g_CamInfo.TargetLookAtPosition.getY() + PLAYER_OFFSET_Y);

        // 目標座標の算出
        g_CamInfo.TargetPosition.setX(0.0f);
        g_CamInfo.TargetPosition.setZ(cos(g_CamInfo.VAngle) * PLAYER_DISTANCE);
        g_CamInfo.TargetPosition.setY(sin(g_CamInfo.VAngle) * PLAYER_DISTANCE);

        g_CamInfo.TargetPosition.setX(-sin(g_CamInfo.HAngle) * g_CamInfo.TargetPosition.getZ());
        g_CamInfo.TargetPosition.setZ(cos(g_CamInfo.HAngle) * g_CamInfo.TargetPosition.getZ());

        g_CamInfo.TargetPosition = g_CamInfo.TargetPosition + g_CamInfo.TargetLookAtPosition;

        // カメラの揺らし処理を無視したカメラの座標、注視点座標をそれぞれの目標座標に近づける
        MoveScale = StepTime / MOVE_DIV_NUM;

        TempV = g_CamInfo.TargetPosition - g_CamInfo.OrigPosition;
        TempV = TempV * MoveScale;
        g_CamInfo.OrigPosition = g_CamInfo.OrigPosition + TempV;

        TempV = g_CamInfo.TargetLookAtPosition - g_CamInfo.OrigLookAtPosition;
        TempV = TempV * MoveScale;
        g_CamInfo.OrigLookAtPosition = g_CamInfo.OrigLookAtPosition +  TempV;

        // カメラの揺らし処理を無視したカメラの座標、注視点座標にカメラの揺らし処理による
        // 加算座標を足した座標を現在のカメラの座標、注視点座標とする
        g_CamInfo.Position =
            g_CamInfo.OrigPosition + g_CamInfo.ShakePosition;

        g_CamInfo.LookAtPosition =
            g_CamInfo.OrigLookAtPosition + g_CamInfo.ShakePosition;

        // カメラの座標からカメラの注視点座標へのベクトルを算出
        g_CamInfo.Direction = (g_CamInfo.LookAtPosition - g_CamInfo.Position).normalized();

        // カメラから見て右方向のベクトルを算出
        g_CamInfo.RightDirection =
            g_CamInfo.Direction.cross(CVector3(0.0f, -1.0f, 0.0f)).normalized();

        // カメラの正面方向のＹ成分を抜いたベクトルを算出
        g_CamInfo.FrontDirection =
            CVector3(0.0f, -1.0f, 0.0f).cross(g_CamInfo.RightDirection).normalized();
    }

    // リスナーの位置を変更
    DxLib:: Set3DSoundListenerPosAndFrontPos_UpVecY(
        DxLib::VGet(g_CamInfo.Position.getX(), g_CamInfo.Position.getY(), g_CamInfo.Position.getZ()),
        DxLib::VGet(g_CamInfo.LookAtPosition.getX(), g_CamInfo.LookAtPosition.getY(), g_CamInfo.LookAtPosition.getZ()));
}

// カメラの設定を行う
void Camera::setup(void)
{
    // カメラの写す範囲を設定
    DxLib::SetupCamera_Perspective(CAMERA_FOV * DX_PI_F / 180.0f);
    DxLib::SetCameraNearFar(CAMERA_NEAR_Z, CAMERA_FAR_Z);
    DxLib::SetCameraPositionAndTarget_UpVecY(
        DxLib::VGet(0.f, 1500.f, -1500.f),
        DxLib::VGet(g_CamInfo.LookAtPosition.getX(), g_CamInfo.LookAtPosition.getY(), g_CamInfo.LookAtPosition.getZ()));
    /*DxLib::SetCameraPositionAndTarget_UpVecY(
        DxLib::VGet(g_CamInfo.Position.getX(), g_CamInfo.Position.getY(), g_CamInfo.Position.getZ()), 
        DxLib::VGet(g_CamInfo.LookAtPosition.getX(), g_CamInfo.LookAtPosition.getY(), g_CamInfo.LookAtPosition.getZ()));*/
}

// カメラを揺らす処理を開始する
void Camera::shake(float ShakeWidth, float ShakeTime, float ShakeAngleSpeed)
{
    g_CamInfo.Shake = true;
    g_CamInfo.ShakeWidth = ShakeWidth;
    g_CamInfo.ShakeTimeCounter = 0.0f;
    g_CamInfo.ShakeTime = ShakeTime;
    g_CamInfo.ShakeAngle = 0.0f;
    g_CamInfo.ShakeAngleSpeed = ShakeAngleSpeed;
}

// カメラの向いている方向を取得する
//     戻り値 : カメラの向いている方向のベクトル
CVector3 Camera::getFrontDirection(void)
{
    return g_CamInfo.FrontDirection;
}

// カメラの向いている方向から見て右方向を取得する
//     戻り値 : カメラの向いている方向から見て右方向のベクトルを取得する
CVector3 Camera::getRightDirection(void)
{
    return g_CamInfo.RightDirection;
}
