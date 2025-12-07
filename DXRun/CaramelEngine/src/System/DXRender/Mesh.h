#pragma once
//--------------------------------------------------------------------------------------
// File: Mesh.h
// Mesh: Sdkmeshファイルを扱うメッシュクラスを使用して，3Dモデルを描画する
// Version 2.0
//
// Copyright 2025 K.Konishi
//--------------------------------------------------------------------------------------

namespace CaramelEngine
{
    class MeshRenderer
    {
    private:
        std::unique_ptr<CDXUTSDKMesh> _mesh;

        std::vector<std::unique_ptr<DirectX::XMFLOAT3>> _vertexBuffer;

    public:
        MeshRenderer();
        ~MeshRenderer() = default;

        // メッシュの作成
        HRESULT CreateMesh(LPCWSTR);

        // メッシュ描画
        void Render(DirectX::CXMMATRIX,
            _In_opt_ std::function<void(DirectX::CXMMATRIX, const SDKMESH_MATERIAL*)> setCustomFunction = nullptr, const UINT textureSlot = 0);

        CDXUTSDKMesh* GetSDKMesh() { return _mesh.get(); };

        bool Intersect(const DirectX::XMVECTOR*, const DirectX::XMVECTOR*, DWORD*, float*, float*, float*, float*);
        void ComputeBoundingBox(DirectX::XMVECTOR*, DirectX::XMVECTOR*);
        void ComputeBoundingSphere(DirectX::XMVECTOR*, float*);

    private:
        HRESULT LoadVertexBuffer(LPCWSTR);
    };
}