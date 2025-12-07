#include "pch.h"
#include "DXInclude.h"
#include "DXRenderer2D.h"

bool DXRenderer2D::initialize(HWND hwnd) 
{
    HRESULT hr;
    // D2D1デバイスのファクトリメソッド呼び出し。ここではシングルスレッド用を作成させてる
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (FAILED(hr)) return false;

    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(
        hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top));

    hr = factory->CreateHwndRenderTarget(props, hwndProps, &renderTarget);
    if (FAILED(hr)) return false;

    hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
    return SUCCEEDED(hr);
}

void DXRenderer2D::beginDraw() 
{
    renderTarget->BeginDraw();
}

void DXRenderer2D::endDraw() 
{
    renderTarget->EndDraw();
}

void DXRenderer2D::clear(const D2D1_COLOR_F& color) 
{
    renderTarget->Clear(color);
}

void DXRenderer2D::drawRect(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color) 
{
    brush->SetColor(color);
    renderTarget->FillRectangle(rect, brush);
}

void DXRenderer2D::drawCircle(const D2D1_ELLIPSE& ellipse, const D2D1_COLOR_F& color) 
{
    brush->SetColor(color);
    renderTarget->FillEllipse(ellipse, brush);
}

void DXRenderer2D::drawSprite(ID2D1Bitmap* bitmap, const D2D1_RECT_F& dst, const D2D1_RECT_F* src) 
{
    if (bitmap) {
        renderTarget->DrawBitmap(bitmap, dst, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
    }
}

void DXRenderer2D::shutdown() 
{
    if (brush) brush->Release();
    if (renderTarget) renderTarget->Release();
    if (factory) factory->Release();
    brush = nullptr;
    renderTarget = nullptr;
    factory = nullptr;
}