#pragma once
#include <DirectXMath.h>
#include "CVector3.h"

using namespace DirectX;

namespace CaramelEngine 
{
	/// <summary>
	/// 3D世界を見るカメラとなるクラス
	/// </summary>
	class Camera 
	{
	public:

		// コンストラクタ
		Camera() = default;

		// 視野角、アスペクト比、近平面、遠平面から投影行列を作る
		void setPerspective(float fovY, float aspect, float nearZ, float farZ) 
		{
			_proj = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
		}

		// 視点、注視する対象、上方向ベクトルからビュー行列を作るメソッド
		void lookAt(const CVector3& eye, const CVector3& target, const CVector3& up) 
		{
			_eyePosition = eye;
			_view = XMMatrixLookAtLH(eye.toXMVECTOR(), target.toXMVECTOR(), up.toXMVECTOR());
		}

		// ビュー行列を返すメソッド
		XMMATRIX getViewMatrix() const 
		{
			return _view;
		}

		// 投影行列を返すメソッド
		XMMATRIX getProjMatrix() const 
		{
			return _proj;
		}

		// ビュー行列と、投影行列を結合して返すメソッド
		// カメラから見たスクリーン座標になる。これをGPUにぶん投げることになる
		XMMATRIX getViewProjMatrix() const 
		{
			return XMMatrixMultiply(_view, _proj);
		}

		// ワールド座標からカメラ空間へのZ座標を計算するメソッド
		float computeDepth(const CVector3& worldPosition) const 
		{
			XMVECTOR pos = worldPosition.toXMVECTOR();
			XMVECTOR viewSpace = XMVector3TransformCoord(pos, _view);
			return XMVectorGetZ(viewSpace);
		}

		// 視座標を返すメソッド
		const CVector3& getPosition() const 
		{
			return _eyePosition;
		}

		void SetScreen(const float angle, const float aspect, const float nearZ, const float farZ)
		{
			_proj = XMMatrixPerspectiveFovLH(angle, aspect, nearZ, farZ);
		}

		void MoveView(const XMMATRIX matrix)
		{
			_view = matrix * _view;
		}

	private:
		XMMATRIX _view = XMMatrixIdentity();
		XMMATRIX _proj = XMMatrixIdentity();
		CVector3 _eyePosition;
	};


} // namespace CaramelEngine
