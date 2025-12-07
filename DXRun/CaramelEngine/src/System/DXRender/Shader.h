#pragma once

namespace CaramelEngine
{
    class Shader
    {
    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>  _vertexLayout;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>  _pixelShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>  _pixelShaderTexture;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerLinear;

        Microsoft::WRL::ComPtr<ID3D11Buffer> _cBMatrix;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _cBMaterial;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _cBLighting;

    public:
        Shader();
        ~Shader() = default;

        HRESULT CreateShader(ID3D11Device* const);
        void SetMatrix(ID3D11DeviceContext* const,
            const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMVECTOR&);
        void SetMaterial(ID3D11DeviceContext* const,
            const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const float&);
        void SetLighting(ID3D11DeviceContext* const,
            const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&);
        void SetRenderShader(ID3D11DeviceContext* const, const bool, const UINT textureSlot = 0);
    };
}