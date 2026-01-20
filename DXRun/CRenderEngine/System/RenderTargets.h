#pragma once

namespace CaramelEngine
{
    struct RenderTargetDesc
    {
        uint32_t width = 1;
        uint32_t height = 1;
        DXGI_FORMAT colorFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
        DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        uint32_t sampleCount = 1;
        bool allowUav = false;
        bool srgb = false;
    };

    class GraphicsDevice;

    class IRenderTarget
    {
    public:
        virtual ~IRenderTarget() = default;

        virtual void resize(GraphicsDevice& gfx, uint32_t w, uint32_t h) = 0;

        virtual ID3D11RenderTargetView* rtv() const noexcept = 0;
        virtual ID3D11DepthStencilView* dsv() const noexcept = 0;
        virtual const D3D11_VIEWPORT& viewport() const noexcept = 0;

        virtual uint32_t width() const noexcept = 0;
        virtual uint32_t height() const noexcept = 0;
    };

    class ISwapChainTarget : public IRenderTarget
    {
    public:
        virtual void present(GraphicsDevice& gfx, bool vsync) = 0;
        virtual IDXGISwapChain1* swapChain() const noexcept = 0;
    };

    class SwapChainTarget final : public ISwapChainTarget
    {
    public:
        void initialize(GraphicsDevice& gfx, HWND hwnd, const RenderTargetDesc& desc,
            uint32_t backBufferCount, bool flipPresent);

        void resize(GraphicsDevice& gfx, uint32_t w, uint32_t h) override;

        ID3D11RenderTargetView* rtv() const noexcept override { return _rtv.Get(); }
        ID3D11DepthStencilView* dsv() const noexcept override { return _dsv.Get(); }
        const D3D11_VIEWPORT& viewport() const noexcept override { return _vp; }

        uint32_t width() const noexcept override { return _desc.width; }
        uint32_t height() const noexcept override { return _desc.height; }

        void present(GraphicsDevice& gfx, bool vsync) override;
        IDXGISwapChain1* swapChain() const noexcept override { return _swapChain.Get(); }

    private:
        void createViews(GraphicsDevice& gfx);
        void releaseViews();

        RenderTargetDesc _desc{};
        uint32_t _backBufferCount = 2;
        bool _flipPresent = true;
        HWND _hwnd = nullptr;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> _backBuffer;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _rtv;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> _depth;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _dsv;

        D3D11_VIEWPORT _vp{};
    };

    class OffscreenTarget final : public IRenderTarget
    {
    public:
        void initialize(GraphicsDevice& gfx, const RenderTargetDesc& desc);
        void resize(GraphicsDevice& gfx, uint32_t w, uint32_t h) override;

        ID3D11RenderTargetView* rtv() const noexcept override { return _rtv.Get(); }
        ID3D11DepthStencilView* dsv() const noexcept override { return _dsv.Get(); }
        const D3D11_VIEWPORT& viewport() const noexcept override { return _vp; }

        uint32_t width() const noexcept override { return _desc.width; }
        uint32_t height() const noexcept override { return _desc.height; }

        // post-process input
        ID3D11ShaderResourceView* srv() const noexcept { return _srv.Get(); }

    private:
        void createResources(GraphicsDevice& gfx);
        void releaseResources();

        RenderTargetDesc _desc{};

        Microsoft::WRL::ComPtr<ID3D11Texture2D> _color;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _rtv;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> _depth;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _dsv;

        D3D11_VIEWPORT _vp{};
    };
}
