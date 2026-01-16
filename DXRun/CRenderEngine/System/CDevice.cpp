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