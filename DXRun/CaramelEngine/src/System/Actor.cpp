#include "pch.h"

#include <DirectXMath.h>
using namespace DirectX;
using namespace CaramelEngine;

bool Actor::isInScreen(const std::shared_ptr<CameraBase> const camera) const noexcept
{
    // ワールド座標（Actorの中心）
    const CVector3& pos = _transform->getPosition();

    // ワールド座標 → ベクトル
    XMVECTOR worldPos = XMVectorSet(
        pos.getX(),
        pos.getY(),
        pos.getZ(),
        1.0f);

    // View / Projection 行列をロード
    XMMATRIX view = XMLoadFloat4x4(&camera->getViewMatrix());
    XMMATRIX proj = XMLoadFloat4x4(&camera->getProjectionMatrix());

    // ViewProjection 行列
    XMMATRIX viewProj = XMMatrixMultiply(view, proj);

    // クリップ空間へ変換
    XMVECTOR clipPos = XMVector4Transform(worldPos, viewProj);

    // w <= 0 → カメラの後ろ
    float w = XMVectorGetW(clipPos);
    if (w <= 0.0f)
        return false;

    // NDC（正規化デバイス座標）
    float x = XMVectorGetX(clipPos) / w;
    float y = XMVectorGetY(clipPos) / w;
    float z = XMVectorGetZ(clipPos) / w;

    // DirectX (LH) の NDC 範囲
    return
        x >= -1.0f && x <= 1.0f &&
        y >= -1.0f && y <= 1.0f &&
        z >= 0.0f && z <= 1.0f;
}
