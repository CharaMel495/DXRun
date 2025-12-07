#pragma once
#include <string>
#include <memory>
#include "CVector2.h"

// DX11
#include <d3d11.h>

namespace CaramelEngine
{
	/// <summary>
	/// UV矩形 (0〜1 範囲でテクスチャ内の領域を表す)
	/// 左上 (u, v), 幅, 高さ
	/// </summary>
	struct CUVRect
	{
		float u = 0.f;
		float v = 0.f;
		float width = 1.f;
		float height = 1.f;
	};

	/// <summary>
	/// 1枚のスプライト（テクスチャの一部分＋ピボット）
	/// </summary>
	class Sprite
	{
	public:
		Sprite(const std::string& name,
			ID3D11ShaderResourceView* srv,
			const CUVRect& uvRect = { 0.f, 0.f, 1.f, 1.f },
			const CVector2& pivot = { 0.5f, 0.5f }) noexcept;

		// --- 基本情報 ---
		const std::string& getName() const noexcept { return _name; }

		// --- テクスチャ ---
		ID3D11ShaderResourceView* getSRV() const noexcept { return _srv; }
		void setSRV(ID3D11ShaderResourceView* srv) noexcept { _srv = srv; }

		// --- Atlas名管理 ---
		void setAtlasName(const std::string& name) noexcept { _atlasName = name; }
		const std::string& getAtlasName() const noexcept { return _atlasName; }

		// --- UV情報 ---
		const CUVRect& getUVRect() const noexcept { return _uvRect; }
		void setUVRect(const CUVRect& rect) noexcept { _uvRect = rect; }

		// --- Pivot ---
		const CVector2& getPivot() const noexcept { return _pivot; }
		void setPivot(const CVector2& pivot) noexcept { _pivot = pivot; }

	private:
		std::string _name;							 // 識別用の名前
		std::string _atlasName;						 // どのAtlasに属するか
		ID3D11ShaderResourceView* _srv = nullptr;	 // テクスチャ参照
		CUVRect _uvRect;								 // 使用するUV領域
		CVector2 _pivot;								 // ピボット位置 (0〜1)
	};
}
