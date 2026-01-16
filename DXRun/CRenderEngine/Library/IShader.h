#pragma once
#include "CaramelEngine.h"

namespace CaramelEngine
{
	/// <summary>
	/// シェーダとつなぐクラスのインターフェース
	/// </summary>
	class IShader
	{
	public:

		IShader() = default;
		virtual ~IShader() = default;

		// 描画前に呼び出し、バインドする
		virtual void bind(class CDeviceContext& context) = 0;
		// 定数バッファを提出
		virtual void applyCBuffer(class CDeviceContext& context, const struct RenderCommand& command) = 0;

	protected:

		// 使用する頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
		// 使用するピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader>  _pixelShader;
	};
}