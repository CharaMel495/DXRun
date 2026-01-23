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

HRESULT CDeviceContext::updateConstantBuffer(ID3D11Buffer* buffer, void* data, size_t size) noexcept
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

HRESULT CDeviceContext::setVSConstantBuffer(UINT startSlot, UINT numBuffer, ID3D11Buffer* const* buffer)
{
    if (!_context)
        return E_ABORT;

    _context->VSSetConstantBuffers(startSlot, numBuffer, buffer);

    return S_OK;
}

HRESULT CDeviceContext::setPSConstantBuffer(UINT startSlot, UINT numBuffer, ID3D11Buffer* const* buffer)
{
    if (!_context)
        return E_ABORT;

    _context->PSSetConstantBuffers(startSlot, numBuffer, buffer);

    return S_OK;
}

HRESULT CDeviceContext::setRenderTargets(UINT numViews, ID3D11RenderTargetView* const * rtv, ID3D11DepthStencilView* dsv) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->OMSetRenderTargets(numViews, rtv, dsv);

    return S_OK;
}

HRESULT CDeviceContext::setViewports(UINT numViewPorts, const D3D11_VIEWPORT* viewPorts) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->RSSetViewports(numViewPorts, viewPorts);

    return S_OK;
}

HRESULT CDeviceContext::clearRenderTarget(ID3D11RenderTargetView* rtv, const FLOAT* clearColor) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->ClearRenderTargetView(rtv, clearColor);

    return S_OK;
}

HRESULT CDeviceContext::clearDepthStencil(ID3D11DepthStencilView* dsv, UINT clearFlag, FLOAT depth, UINT8 stencil) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->ClearDepthStencilView(dsv, clearFlag, depth, stencil);

    return S_OK;
}

HRESULT CDeviceContext::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->IASetPrimitiveTopology(topology);

    return S_OK;
}

HRESULT CDeviceContext::setVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* buffers, const UINT* strides, const UINT* offsets) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->IASetVertexBuffers(startSlot, numBuffers, buffers, strides, offsets);

    return S_OK;
}

HRESULT CDeviceContext::setIndexBuffer(ID3D11Buffer* buffer, DXGI_FORMAT format, UINT offset) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->IASetIndexBuffer(buffer, format, offset);

    return S_OK;
}

HRESULT CDeviceContext::drawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation) noexcept
{
    if (!_context)
        return E_ABORT;

    _context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);

    return S_OK;
}