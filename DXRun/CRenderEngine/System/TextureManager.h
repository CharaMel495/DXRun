#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <wincodec.h>
#include <unordered_map>
#include <string>

namespace CaramelEngine
{
    class TextureManager
    {
    public:
        static TextureManager& getInstance()
        {
            static TextureManager instance;
            return instance;
        }

        void initialize(ID3D11Device* device, ID3D11DeviceContext* context,
            bool generateMips = true, bool treatAsSRGB = true);
        void shutdown();

        ID3D11Device* getDevice() const noexcept { return _device.Get(); }
        ID3D11DeviceContext* getContext() const noexcept { return _context.Get(); }

        // --- 改良 ---
        struct TextureInfo
        {
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
            UINT width = 0;
            UINT height = 0;
            DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
        };

        // ファイルパスをキーにロード（キャッシュ付き）
        const TextureInfo* loadTexture(const std::string& path);

        // 取得
        const TextureInfo* getTextureInfo(const std::string& path) const;

        // 破棄
        void clear();

    private:
        bool loadFromWIC(const wchar_t* filename,
            Microsoft::WRL::ComPtr<ID3D11Texture2D>& outTex,
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& outSRV,
            UINT& outWidth, UINT& outHeight, DXGI_FORMAT& outFormat);

        Microsoft::WRL::ComPtr<ID3D11Device> _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> _context;
        Microsoft::WRL::ComPtr<IWICImagingFactory> _wicFactory;

        bool _generateMips = true;
        bool _srgb = true;

        std::unordered_map<std::string, TextureInfo> _textures;
    };
}