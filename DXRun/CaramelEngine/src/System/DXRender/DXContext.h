#pragma once
#include "DXInclude.h"

class DXContext 
{
public:
    bool initialize(HWND hwnd);
    void shutdown();

    ID3D11Device* getDevice();
    ID3D11DeviceContext* getContext();

private:
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
};