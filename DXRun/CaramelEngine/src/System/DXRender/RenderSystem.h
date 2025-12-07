#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_3.h>  // CreateDXGIFactory2 / Flip-Model 用
#include <cstdint>

namespace CaramelEngine
{
    /// <summary>
    /// 描画そのものを司るクラス
    /// </summary>
    class RenderSystem 
    {
    public:
        static RenderSystem& getInstance()
        {
            static RenderSystem instance;
            return instance;
        }

        // 初期化/終了
        bool initialize(HWND hwnd, int width, int height, bool enableDebug = false);
        void shutdown();

        // 画面サイズ変更
        bool resize(int width, int height);

        // フレーム制御
        void beginFrame(const float clearColor[4]);
        void endFrame(bool vsync = true);

        // アクセサ
        ID3D11Device* getDevice()  const { return _device.Get(); }
        ID3D11DeviceContext* getContext() const { return _context.Get(); }
        ID3D11RenderTargetView* getBackbufferRTV() const { return _rtv.Get(); }
        ID3D11DepthStencilView* getDSV() const { return _dsv.Get(); }

        int  width()  const { return _width; }
        int  height() const { return _height; }

    private:
        bool createDeviceAndSwapChain(HWND hwnd, int width, int height, bool enableDebug);
        bool createBackbufferViews(int width, int height);
        void releaseBackbufferViews();
        void setDefaultViewport(int width, int height);

    private:
        // D3D core
        Microsoft::WRL::ComPtr<ID3D11Device>            _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     _context;
        Microsoft::WRL::ComPtr<IDXGISwapChain1>         _swapchain;   // Flip-Model
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  _rtv;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  _dsv;

        // 深度用バッファ
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         _depthTex;

        // 状態
        int _width = 0;
        int _height = 0;
    };
}