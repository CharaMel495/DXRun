#pragma once

#include "DxLib.h"
#include "CaramelEngine.h"
#include "ITargetProvider.h"

// カメラの手前クリップ距離
#define CAMERA_NEAR_Z       (20.0f)

// カメラの奥クリップ距離
#define CAMERA_FAR_Z        (25000.0f)

class Camera
{
public:

    inline Camera() noexcept = default;

    virtual ~Camera() noexcept {}

    // カメラの処理を初期化する
    void initialize(void);

    // カメラの状態推移処理を実行する
    void update(float StepTime);

    // カメラの設定を行う
    void setup(void);

    // カメラを揺らす処理を開始する
    void shake(float ShakeWidth, float ShakeTime, float ShakeAngleSpeed);

    // カメラの向いている方向を取得する
    // 戻り値 : カメラの向いている方向のベクトル
    CVector3 getFrontDirection(void);

    // カメラの向いている方向から見て右方向を取得する
    //     戻り値 : カメラの向いている方向から見て右方向のベクトルを取得する
    CVector3 getRightDirection(void);

    void setTarget(std::weak_ptr<ITargetProvider> target) noexcept { _target = target; }
private:

    std::weak_ptr<ITargetProvider> _target;
};