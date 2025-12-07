#include "pch.h"
#include "Shader.h"
#include "ReadData.h"

using namespace DirectX;

namespace
{
    struct CBMatrix
    {
        XMMATRIX WorldViewProjection;
        XMVECTOR LightDirection;
        XMVECTOR EyePosition;
    };
    static_assert((sizeof(CBMatrix) % 16) == 0,
        "Constant buffer must always be 16-byte aligned");

    struct CBMaterial
    {
        XMFLOAT4 MaterialAmbient;
        XMFLOAT4 MaterialDiffuse;
        XMFLOAT4 MaterialSpeqular;
        XMFLOAT4 MaterialPower;
    };
    static_assert((sizeof(CBMaterial) % 16) == 0,
        "Constant buffer must always be 16-byte aligned");

    struct CBLighting
    {
        XMFLOAT4 LightAmbient;
        XMFLOAT4 LightDiffuse;
        XMFLOAT4 LightSpeqular;
    };
    static_assert((sizeof(CBLighting) % 16) == 0,
        "Constant buffer must always be 16-byte aligned");

    LPCWSTR VertexFilename = L"Shader\\cso\\BasicVS.cso";
    LPCWSTR PixelFilename = L"Shader\\cso\\BasicPS.cso";
    LPCWSTR PixelTextureFilename = L"Shader\\cso\\TexturePS.cso";
}

namespace CaramelEngine
{
    //--------------------------------------------------------------------------------------
    Shader::Shader() :
        _vertexShader(nullptr), _vertexLayout(nullptr), _pixelShader(nullptr),
        _pixelShaderTexture(nullptr), _samplerLinear(nullptr),
        _cBMatrix(nullptr), _cBMaterial(nullptr), _cBLighting(nullptr)
    {
    }

    //--------------------------------------------------------------------------------------
    HRESULT Shader::CreateShader(ID3D11Device* const device)
    {
        HRESULT hr = S_OK;
        // Load the vertex shader
        auto vertexShaderBlob = DX::ReadData(VertexFilename);
        // Create the vertex shader
        hr = device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(),
            nullptr, _vertexShader.ReleaseAndGetAddressOf());
        if (FAILED(hr)) {
            vertexShaderBlob.clear();
            return hr;
        }

        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 0
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 3
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 5
        };
        // Create the input layout
        hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexShaderBlob.data(),
            vertexShaderBlob.size(), _vertexLayout.ReleaseAndGetAddressOf());
        vertexShaderBlob.clear();
        if (FAILED(hr))
            return hr;

        // Load the pixel shader
        auto pixelShaderBlob = DX::ReadData(PixelFilename);
        // Create the pixel shader
        hr = device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
            nullptr, _pixelShader.ReleaseAndGetAddressOf());
        pixelShaderBlob.clear();
        if (FAILED(hr))
            return hr;

        // Load the pixel shader
        pixelShaderBlob = DX::ReadData(PixelTextureFilename);
        // Create the pixel shader
        hr = device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
            nullptr, _pixelShaderTexture.ReleaseAndGetAddressOf());
        pixelShaderBlob.clear();
        if (FAILED(hr))
            return hr;

        // Create the sampler state
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        V_RETURN(hr = device->CreateSamplerState(&samplerDesc, _samplerLinear.ReleaseAndGetAddressOf()));

        // Create the constant buffers
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;
        bd.ByteWidth = sizeof(CBMatrix);
        V_RETURN(device->CreateBuffer(&bd, nullptr, _cBMatrix.ReleaseAndGetAddressOf()));
        bd.ByteWidth = sizeof(CBMaterial);
        V_RETURN(device->CreateBuffer(&bd, nullptr, _cBMaterial.ReleaseAndGetAddressOf()));
        bd.ByteWidth = sizeof(CBLighting);
        V_RETURN(device->CreateBuffer(&bd, nullptr, _cBLighting.ReleaseAndGetAddressOf()));

        return S_OK;
    }

    //--------------------------------------------------------------------------------------
    void Shader::SetMatrix(ID3D11DeviceContext* const context,
        const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, const XMVECTOR& lightDirection)
    {
        // ローカル-射影変換行列
        auto matWVP = world * view * projection;
        matWVP = XMMatrixTranspose(matWVP);

        // ライトの方向(物体のローカル座標での)
        // ワールド行列の逆行列を計算
        XMVECTOR vec = {};
        auto mat = XMMatrixInverse(&vec, world);
        // ワールド座標のライトをローカル座標でのライトに計算
        auto light = XMVector4Transform(lightDirection, mat);
        // 正規化
        light = XMVector3Normalize(light);

        // 視点
        mat = XMMatrixInverse(&vec, (world * view));
        XMVECTOR eyeOrg = { 0, 0, 0, 1 };
        auto eye = XMVector4Transform(eyeOrg, mat);

        // Set Constant Buffer
        D3D11_MAPPED_SUBRESOURCE MappedResource = {};
        context->Map(_cBMatrix.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
        auto cbMatrix = (CBMatrix*)MappedResource.pData;
        cbMatrix->WorldViewProjection = matWVP;
        cbMatrix->LightDirection = light;
        cbMatrix->EyePosition = eye;
        context->Unmap(_cBMatrix.Get(), 0);
    }

    //--------------------------------------------------------------------------------------
    void Shader::SetMaterial(ID3D11DeviceContext* const context,
        const XMFLOAT4& materialAmbient, const XMFLOAT4& materialDiffuse, const XMFLOAT4& materialSpeqular, const float& power)
    {
        // Set Constant Buffer
        D3D11_MAPPED_SUBRESOURCE MappedResource = {};
        context->Map(_cBMaterial.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
        auto cbMaterial = (CBMaterial*)MappedResource.pData;
        //cbMaterial->meshAmbient = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
        cbMaterial->MaterialAmbient = materialAmbient;
        cbMaterial->MaterialDiffuse = materialDiffuse;
        cbMaterial->MaterialSpeqular = materialSpeqular;
        cbMaterial->MaterialPower = { power, 0.0f, 0.0f, 0.0f };
        context->Unmap(_cBMaterial.Get(), 0);
    }

    //--------------------------------------------------------------------------------------
    void Shader::SetLighting(ID3D11DeviceContext* const context,
        const XMFLOAT4& lightAmbient, const XMFLOAT4& lightDiffuse, const XMFLOAT4& lightSpeqular)
    {
        // Set Constant Buffer
        D3D11_MAPPED_SUBRESOURCE MappedResource = {};
        context->Map(_cBLighting.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
        auto cbLight = (CBLighting*)MappedResource.pData;
        cbLight->LightAmbient = lightAmbient;
        cbLight->LightDiffuse = lightDiffuse;
        cbLight->LightSpeqular = lightSpeqular;
        context->Unmap(_cBLighting.Get(), 0);
    }

    //--------------------------------------------------------------------------------------
    void Shader::SetRenderShader(ID3D11DeviceContext* const context, const bool texture, const UINT textureSlot)
    {
        // Set the input layout
        context->IASetInputLayout(_vertexLayout.Get());

        // Set render resources
        context->VSSetShader(_vertexShader.Get(), nullptr, 0);
        context->VSSetConstantBuffers(0, 1, _cBMatrix.GetAddressOf());
        context->VSSetConstantBuffers(1, 1, _cBMaterial.GetAddressOf());
        context->VSSetConstantBuffers(2, 1, _cBLighting.GetAddressOf());

        // テクスチャをシェーダに渡す
        if (texture) {
            context->PSSetShader(_pixelShaderTexture.Get(), nullptr, 0);
            context->PSSetSamplers(textureSlot, 1, _samplerLinear.GetAddressOf());
        }
        else {
            context->PSSetShader(_pixelShader.Get(), nullptr, 0);
        }
    }
}