#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// GPUへドローコールを飛ばすクラス
	/// </summary>
	class CDeviceContext
	{
	public:
		CDeviceContext() = delete;
		explicit CDeviceContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) noexcept : _context(context) {}
		~CDeviceContext() = default;

		// コンテキストの再アタッチ(必要な場合は)
		void attachContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context) noexcept { _context = context; }

		HRESULT setVertexShader(ID3D11VertexShader* vs) noexcept;
		HRESULT setPixelShader(ID3D11PixelShader* ps) noexcept;
		HRESULT setInputLayout(ID3D11InputLayout* layout) noexcept;

		HRESULT UpdateConstantBuffer(ID3D11Buffer* buffer, void* data, size_t size) noexcept;

		HRESULT DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation) noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
	};
}