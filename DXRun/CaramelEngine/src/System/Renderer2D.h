#pragma once

namespace CaramelEngine
{
	namespace Renderer2D
	{
		/// <summary>
		/// 中心座標とサイズで矩形を描画
		/// </summary>
		void drawRectangle(const CVector3& center, const CVector3& size, const Color& color);

		/// <summary>
		/// 中心座標と半径で円を描画
		/// </summary>
		void drawCircle(const CVector3& center, float radius, const Color& color);

		/// <summary>
		/// テクスチャIDでスプライトを描画（中心座標基準）
		/// </summary>
		void drawSprite(int textureId, const CVector3& center, const CVector3& size, const Color& color);
	}
}
