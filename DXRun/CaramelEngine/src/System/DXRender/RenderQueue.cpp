#include "pch.h"
#include "RenderQueue.h"
#include "RenderSystem.h"
#include "SpriteVertex.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

namespace CaramelEngine
{
    // ---------------------------------------------------------------
    // 初期化
    // ---------------------------------------------------------------
    void RenderQueue::initialize(ID3D11Device* device)
    {
        // 頂点データ（1枚の四角）
        SpriteVertex vertices[] =
        {
            {{-0.5f,  0.5f, 0.0f, 1.0f}, {0.f, 0.f}, {1,1,1,1}},
            {{ 0.5f,  0.5f, 0.0f, 1.0f}, {1.f, 0.f}, {1,1,1,1}},
            {{-0.5f, -0.5f, 0.0f, 1.0f}, {0.f, 1.f}, {1,1,1,1}},
            {{ 0.5f, -0.5f, 0.0f, 1.0f}, {1.f, 1.f}, {1,1,1,1}},
        };

        D3D11_BUFFER_DESC vbd{};
        vbd.ByteWidth = static_cast<UINT>(sizeof(vertices));
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = vertices;

        device->CreateBuffer(&vbd, &init, _vertexBuffer.ReleaseAndGetAddressOf());

        // 定数バッファ
        D3D11_BUFFER_DESC cbd{};
        cbd.ByteWidth = sizeof(SpriteVSConstant);
        cbd.Usage = D3D11_USAGE_DEFAULT;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        HRESULT hr = device->CreateBuffer(&cbd, nullptr, _constBuffer.ReleaseAndGetAddressOf());
        if (FAILED(hr))
            return;

        // シェーダコンパイル
        Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;
        hr = D3DCompileFromFile(L"Shader\\SpriteVS.hlsl", nullptr, nullptr, "main", "vs_4_0", 0, 0, &vsBlob, &errBlob);
        if (FAILED(hr))
            return;
        hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, _vs.GetAddressOf());
        if (FAILED(hr))
            return;
        hr = D3DCompileFromFile(L"Shader\\SpritePS.hlsl", nullptr, nullptr, "main", "ps_4_0", 0, 0, &psBlob, &errBlob);
        if (FAILED(hr))
            return;
        device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, _ps.GetAddressOf());
        if (FAILED(hr))
            return;

        //hr = D3DReadFileToBlob(L"Shader\\SpriteVS.cso", &vsBlob);
        //if (FAILED(hr))
        //    return;
        //hr = D3DReadFileToBlob(L"Shader\\SpritePS.cso", &psBlob);
        //if (FAILED(hr))
        //    return;

        // 入力レイアウト
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SpriteVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(SpriteVertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,offsetof(SpriteVertex,color),   D3D11_INPUT_PER_VERTEX_DATA, 0},
        };
        hr = device->CreateInputLayout(layout, _countof(layout),
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
            _inputLayout.ReleaseAndGetAddressOf());
        if (FAILED(hr))
            return;

        // サンプラ
        D3D11_SAMPLER_DESC samp{};
        samp.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samp.AddressU = samp.AddressV = samp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samp.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samp.MinLOD = 0;
        samp.MaxLOD = D3D11_FLOAT32_MAX;
        device->CreateSamplerState(&samp, _sampler.ReleaseAndGetAddressOf());

        // ブレンド
        D3D11_BLEND_DESC blend{};
        blend.RenderTarget[0].BlendEnable = TRUE;
        blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        device->CreateBlendState(&blend, _blend.ReleaseAndGetAddressOf());
    }

    // ---------------------------------------------------------------
    // flush
    // ---------------------------------------------------------------
    void RenderQueue::flush(RenderPassType pass)
    {
        auto& list = _commands[static_cast<size_t>(pass)];
        if (list.empty())
            return;

        auto& renderSys = RenderSystem::getInstance();
        auto context = renderSys.getContext();

        UINT stride = sizeof(SpriteVertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(_inputLayout.Get());
        context->VSSetShader(_vs.Get(), NULL, 0);
        context->PSSetShader(_ps.Get(), NULL, 0);
        context->PSSetSamplers(0, 1, _sampler.GetAddressOf());

        float blendFactor[4] = { 0,0,0,0 };
        context->OMSetBlendState(_blend.Get(), blendFactor, 0xffffffff);

        // 正射影行列（ピクセル基準、左上原点）
        XMMATRIX view = XMMatrixIdentity();
        XMMATRIX proj = XMMatrixOrthographicOffCenterRH(0, 1280, 720, 0, 0, 1);

        for (auto& cmd : list)
        {
            if (!cmd.srv)
                continue;

            context->PSSetShaderResources(0, 1, &cmd.srv);

            // --- 定数バッファ構築 ---
            SpriteVSConstant cbData{};
            cbData.world = XMMatrixTranspose(
                XMMatrixScaling(cmd.scale.getX(), cmd.scale.getY(), cmd.scale.getZ()) *
                XMMatrixRotationZ(cmd.rotation) *
                XMMatrixTranslation(cmd.position.getX(), cmd.position.getY(), cmd.position.getZ())
            );
            cbData.viewProj = XMMatrixTranspose(view * proj);
            // world行列
            cbData.uvOffset = cmd.uvOffset.toXMFLOAT2();
            cbData.uvScale = cmd.uvScale.toXMFLOAT2();
            cbData.color = cmd.color.toXMFLOAT4();

            // バッファ更新
            context->UpdateSubresource(_constBuffer.Get(), 0, nullptr, &cbData, 0, 0);
            context->VSSetConstantBuffers(0, 1, _constBuffer.GetAddressOf());

            // 描画
            context->Draw(4, 0);

            XMFLOAT4X4 debugViewProj;
            XMStoreFloat4x4(&debugViewProj, cbData.viewProj);
            char buf[512];
            sprintf_s(buf,
                "viewProj:\n"
                "%.3f %.3f %.3f %.3f\n"
                "%.3f %.3f %.3f %.3f\n"
                "%.3f %.3f %.3f %.3f\n"
                "%.3f %.3f %.3f %.3f\n",
                debugViewProj._11, debugViewProj._12, debugViewProj._13, debugViewProj._14,
                debugViewProj._21, debugViewProj._22, debugViewProj._23, debugViewProj._24,
                debugViewProj._31, debugViewProj._32, debugViewProj._33, debugViewProj._34,
                debugViewProj._41, debugViewProj._42, debugViewProj._43, debugViewProj._44);
            OutputDebugStringA(buf);

        }

        // SRV解除
        ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
        context->PSSetShaderResources(0, 1, nullSRV);
    }


    void RenderQueue::flushAll()
    {
        // 各描画パスを順に描画
        flush(RenderPassType::DepthPrepass);
        flush(RenderPassType::Opaque);
        flush(RenderPassType::Transparent);
        flush(RenderPassType::Sprite);
        flush(RenderPassType::UI);

        clear();
    }
}