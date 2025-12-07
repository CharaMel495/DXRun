#pragma once
#include <cstdint>
#include <cmath>

namespace CaramelEngine
{
	/// <summary>
	/// RGBAカラーデータ（0-255）＋便利関数
	/// </summary>
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		// --- コンストラクタ ---

		constexpr Color() noexcept : r(255), g(255), b(255), a(255) {}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept
			: r(r), g(g), b(b), a(a) {
		}

		// float(0.0f-1.0f) → Color変換
		static constexpr Color fromFloat(float r, float g, float b, float a = 1.0f) noexcept
		{
			return Color(
				static_cast<uint8_t>(r * 255.0f),
				static_cast<uint8_t>(g * 255.0f),
				static_cast<uint8_t>(b * 255.0f),
				static_cast<uint8_t>(a * 255.0f)
			);
		}

		// float(0.0f-1.0f)の表現に変換
		inline void toFloat(float& outR, float& outG, float& outB, float& outA) const noexcept
		{
			outR = r / 255.0f;
			outG = g / 255.0f;
			outB = b / 255.0f;
			outA = a / 255.0f;
		}

		// --- ブレンド計算 ---
		// src over dst アルファブレンド
		static Color blend(const Color& src, const Color& dst) noexcept
		{
			float srcA = src.a / 255.0f;
			float dstA = dst.a / 255.0f;
			float outA = srcA + dstA * (1.0f - srcA);

			if (outA == 0.0f) return Color(0, 0, 0, 0);

			auto blendChannel = [&](uint8_t sc, uint8_t dc) -> uint8_t {
				return static_cast<uint8_t>(
					((sc * srcA + dc * dstA * (1.0f - srcA)) / outA) + 0.5f
					);
				};

			return Color(
				blendChannel(src.r, dst.r),
				blendChannel(src.g, dst.g),
				blendChannel(src.b, dst.b),
				static_cast<uint8_t>(outA * 255.0f + 0.5f)
			);
		}

		// --- DirectXMath変換追加 ---
		inline DirectX::XMFLOAT4 toXMFLOAT4() const noexcept
		{
			return 
			{
				r / 255.0f,
				g / 255.0f,
				b / 255.0f,
				a / 255.0f
			};
		}

		// --- プリセットカラー ---
		static constexpr Color White() { return Color(255, 255, 255, 255); }
		static constexpr Color Black() { return Color(0, 0, 0, 255); }
		static constexpr Color Transparent() { return Color(0, 0, 0, 0); }
		static constexpr Color Red() { return Color(255, 0, 0, 255); }
		static constexpr Color Green() { return Color(0, 255, 0, 255); }
		static constexpr Color Blue() { return Color(0, 0, 255, 255); }
	};

}
