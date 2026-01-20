#pragma once

namespace CaramelEngine
{
	class CDevice final
	{
	public:
		CDevice() noexcept = delete;
		CDevice(Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept : _device(device) {}
		~CDevice() noexcept = default;

		// 万が一使用デバイスが切り替わった際に再アタッチする為のメソッド
		void attachDevice(Microsoft::WRL::ComPtr<ID3D11Device> device) noexcept { _device = device; }
		ID3D11Device* getNative() const noexcept { return _device.Get(); }

		HRESULT createBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData, ID3D11Buffer** buffer) const noexcept;
		HRESULT createVertexShader(const void* bytecode,size_t bytecodeLength, ID3D11VertexShader** vs) const noexcept;
		HRESULT createPixelShader(const void* bytecode,size_t bytecodeLength, ID3D11PixelShader** ps) const noexcept;
		HRESULT createInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements,UINT elementCount,const void* shaderBytecode, size_t bytecodeLength, ID3D11InputLayout** layout) const noexcept;
		HRESULT createSamplerState(const D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state) const noexcept;
		HRESULT createTexture2d(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture2D** tex) const noexcept;
		HRESULT createRenderTargetView(ID3D11Resource* res, const D3D11_RENDER_TARGET_VIEW_DESC* desc, ID3D11RenderTargetView** rtv) const noexcept;
		HRESULT createDepthStencilView(ID3D11Resource* res, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc, ID3D11DepthStencilView** dsv) const noexcept;
		HRESULT createShaderResourceView(ID3D11Resource* res, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc, ID3D11ShaderResourceView** srv) const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
	};
}