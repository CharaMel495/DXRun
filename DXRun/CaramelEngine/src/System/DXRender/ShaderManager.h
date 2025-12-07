// ShaderManager.h
#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <string>
#include <unordered_map>

namespace CaramelEngine
{

    struct ShaderProgram 
    {
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>  ps;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>  inputLayout;
    };

    class ShaderManager {
    public:
        void Initialize(ID3D11Device* device);
        bool LoadVSPS(const std::wstring& key,
            const void* vsBytes, size_t vsSize,
            const void* psBytes, size_t psSize,
            const D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutCount);
        const ShaderProgram* Get(const std::wstring& key) const;

    private:
        ID3D11Device* m_device = nullptr;
        std::unordered_map<std::wstring, ShaderProgram> m_programs;
    };

}