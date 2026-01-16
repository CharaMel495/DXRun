#include "pch.h"
#include "CDeviceContext.h"

HRESULT CDeviceContext::setVertexShader(ID3D11VertexShader* vs) noexcept
{
    if (!_context)
        return E_ABORT;

	_context->VSSetShader(vs, nullptr, 0);

    return S_OK;
}

HRESULT CDeviceContext::setPixelShader(ID3D11PixelShader* ps) noexcept
{
    if (!_context)
        return E_ABORT;

	_context->PSSetShader(ps, nullptr, 0);

    return S_OK;
}

HRESULT CDeviceContext::setInputLayout(ID3D11InputLayout* layout) noexcept
{
    if (!_context)
        return E_ABORT;

	_context->IASetInputLayout(layout);

    return S_OK;
}

HRESULT CDeviceContext::UpdateConstantBuffer(ID3D11Buffer* buffer, void* data, size_t size) noexcept
{
    if (!_context)
        return E_ABORT;

    // Lock the constant buffer so it can be written to.
    D3D11_MAPPED_SUBRESOURCE mappedResource{};
    DX::ThrowIfFailed(
        _context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
    );

    // Get a pointer to the data in the constant buffer.
    memcpy(mappedResource.pData, data, size);

    // Unlock the constant buffer.
    _context->Unmap(buffer, 0);

    return S_OK;
}

HRESULT CDeviceContext::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

    return S_OK;
}