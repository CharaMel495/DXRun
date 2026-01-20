#include "pch.h"
#include "CDevice.h"

HRESULT CDevice::createBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData, ID3D11Buffer** buffer) const noexcept
{
    if (!_device)
        return E_ABORT;

    return  _device->CreateBuffer(desc, initData, buffer);
}

HRESULT CDevice::createVertexShader(const void* bytecode, size_t bytecodeLength, ID3D11VertexShader** vs) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateVertexShader(bytecode, bytecodeLength, nullptr, vs);
}

HRESULT CDevice::createPixelShader(const void* bytecode, size_t bytecodeLength, ID3D11PixelShader** ps) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreatePixelShader(bytecode, bytecodeLength, nullptr, ps);
}

HRESULT CDevice::createInputLayout(const D3D11_INPUT_ELEMENT_DESC* elements, UINT elementCount, const void* shaderBytecode, size_t bytecodeLength, ID3D11InputLayout** layout) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateInputLayout(elements, elementCount, shaderBytecode, bytecodeLength, layout);
}

HRESULT CDevice::createSamplerState(const D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateSamplerState(desc, state);
}

HRESULT CDevice::createTexture2d(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* initData, ID3D11Texture2D** tex) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateTexture2D(desc, initData, tex);
}

HRESULT CDevice::createRenderTargetView(ID3D11Resource* res, const D3D11_RENDER_TARGET_VIEW_DESC* desc, ID3D11RenderTargetView** rtv) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateRenderTargetView(res, desc, rtv);
}

HRESULT CDevice::createDepthStencilView(ID3D11Resource* res, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc, ID3D11DepthStencilView** dsv) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateDepthStencilView(res, desc, dsv);
}

HRESULT CDevice::createShaderResourceView(ID3D11Resource* res, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc, ID3D11ShaderResourceView** srv) const noexcept
{
    if (!_device)
        return E_ABORT;

    return _device->CreateShaderResourceView(res, desc, srv);
}