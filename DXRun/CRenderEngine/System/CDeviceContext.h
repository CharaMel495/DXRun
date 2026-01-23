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
		ID3D11DeviceContext* getNative() const noexcept { return _context.Get(); }

		HRESULT setVertexShader(ID3D11VertexShader* vs) noexcept;
		HRESULT setPixelShader(ID3D11PixelShader* ps) noexcept;
		HRESULT setInputLayout(ID3D11InputLayout* layout) noexcept;

		HRESULT updateConstantBuffer(ID3D11Buffer* buffer, void* data, size_t size) noexcept;
		HRESULT setVSConstantBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* buffer);
		HRESULT setPSConstantBuffer(UINT startSlot, UINT numBuffer, ID3D11Buffer* const* buffer);

		HRESULT setRenderTargets(UINT numViews, ID3D11RenderTargetView* const* rtv, ID3D11DepthStencilView* dsv) noexcept;
		HRESULT setViewports(UINT numViewPorts, const D3D11_VIEWPORT* viewPorts) noexcept;
		HRESULT clearRenderTarget(ID3D11RenderTargetView* rtv, const FLOAT* clearColor) noexcept;
		HRESULT clearDepthStencil(ID3D11DepthStencilView* dsv, UINT clearFlag, FLOAT depth, UINT8 stencil) noexcept;

		HRESULT setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept;
		HRESULT setVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* buffers, const UINT* strides, const UINT* offsets) noexcept;
		HRESULT setIndexBuffer(ID3D11Buffer* buffer, DXGI_FORMAT format,	 UINT offset) noexcept;
		HRESULT drawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation) noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
	};
}