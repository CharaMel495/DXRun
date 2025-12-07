#include "pch.h"
#include "TextureManager.h"
#include <vector>

using Microsoft::WRL::ComPtr;

namespace CaramelEngine
{

    void TextureManager::initialize(ID3D11Device* device, ID3D11DeviceContext* context,
        bool generateMips, bool treatAsSRGB)
    {
        _device = device;
        _context = context;
        _generateMips = generateMips;
        _srgb = treatAsSRGB;

        // WICファクトリは1回だけ生成
        if (!_wicFactory) 
        {
            CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(_wicFactory.ReleaseAndGetAddressOf()));
        }

    }

    void TextureManager::shutdown()
    {
        clear(); 
        
        
        _textures.clear();
        _context.Reset();
        _device.Reset();
        _wicFactory.Reset();
    }

    const TextureManager::TextureInfo* TextureManager::loadTexture(const std::string& path)
    {
        // キャッシュチェック
        auto it = _textures.find(path);
        if (it != _textures.end())
            return &it->second;

        // --- WIC読み込み ---
        std::wstring wpath(path.begin(), path.end());
        TextureInfo info;
        if (!loadFromWIC(wpath.c_str(), info.texture, info.srv, info.width, info.height, info.format))
            return nullptr;

        // 登録
        _textures[path] = std::move(info);
        return &_textures[path];
    }

    const TextureManager::TextureInfo* TextureManager::getTextureInfo(const std::string& path) const
    {
        auto it = _textures.find(path);
        return (it != _textures.end()) ? &it->second : nullptr;
    }

    void TextureManager::clear()
    {
        _textures.clear();
    }

    bool TextureManager::loadFromWIC(
        const wchar_t* filename,
        ComPtr<ID3D11Texture2D>& outTex,
        ComPtr<ID3D11ShaderResourceView>& outSRV,
        UINT& outWidth,
        UINT& outHeight,
        DXGI_FORMAT& outFormat)
    {
        if (!_device || !_context || !_wicFactory) 
            return false;

        // --- 1) 画像を WIC で 32bpp BGRA にデコード ---
        ComPtr<IWICBitmapDecoder> decoder;
        ComPtr<IWICBitmapFrameDecode> frame;
        ComPtr<IWICFormatConverter> converter;

        // ここでファイルを開く
        HRESULT hr = _wicFactory->CreateDecoderFromFilename(
            filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr)) 
            return false;

        // 画像の最初のフレームを取得する
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) 
            return false;

        // 画像フォーマットを32bitBGRAに変換
        hr = _wicFactory->CreateFormatConverter(&converter);
        if (FAILED(hr)) 
            return false;

        hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA,
            WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
        if (FAILED(hr))
            return false;

        // サイズを取得
        UINT w = 0, h = 0;
        converter->GetSize(&w, &h);
        if (w == 0 || h == 0) 
            return false;

        const UINT stride = w * 4; // BGRA8
        std::vector<BYTE> pixels;
        pixels.resize(static_cast<size_t>(stride) * h);

        hr = converter->CopyPixels(nullptr, stride, static_cast<UINT>(pixels.size()), pixels.data());
        if (FAILED(hr)) 
            return false;

        // --- 2) D3D11 Texture2D を作る ---
        DXGI_FORMAT format = _srgb ? DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
            : DXGI_FORMAT_B8G8R8A8_UNORM;

        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = w;
        texDesc.Height = h;
        texDesc.MipLevels = _generateMips ? 0 : 1; // 0: 全mip自動
        texDesc.ArraySize = 1;
        texDesc.Format = format;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
            (_generateMips ? D3D11_BIND_RENDER_TARGET : 0);
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = _generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

        // base level を一気に詰める（MipLevels=0 の場合でも OK）
        D3D11_SUBRESOURCE_DATA init = {};
        init.pSysMem = pixels.data();
        init.SysMemPitch = stride;

        // 以下のステップの意図: GENERATE_MIPSを使う場合、ここで初期データを渡すのではなく
        // 「MipLevels=0 のテクスチャを空で作成→UpdateSubresourceでレベル0を書き込み」
        // の方が確実（ドライバによっては初期データ+自動mipsが非対応なことがある）
        // → 安全策として2ステップで実装する

        // 2-1) 空のテクスチャ作成
        ComPtr<ID3D11Texture2D> tex;
        hr = _device->CreateTexture2D(&texDesc, nullptr, &tex);
        if (FAILED(hr)) return false;

        // 2-2) レベル0に転送
        _context->UpdateSubresource(tex.Get(), 0, nullptr, pixels.data(), stride, 0);

        // --- 3) SRV を作る ---
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = _generateMips ? -1 : 1;

        ComPtr<ID3D11ShaderResourceView> srv;
        hr = _device->CreateShaderResourceView(tex.Get(), &srvDesc, &srv);
        if (FAILED(hr)) 
            return false;

        // --- 4) ミップ生成（必要に応じて） ---
        if (_generateMips) 
        {
            _context->GenerateMips(srv.Get());
        }

        outTex = std::move(tex);
        outSRV = std::move(srv);

        outWidth = w;
        outHeight = h;
        outFormat = format;
        return true;
    }
}