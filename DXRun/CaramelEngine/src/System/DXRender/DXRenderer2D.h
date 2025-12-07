#pragma once
#include "DXInclude.h"

class DXRenderer2D 
{
public:
    bool initialize(HWND hwnd);
    void beginDraw();
    void endDraw();
    void clear(const D2D1_COLOR_F& color);

    void drawRect(const D2D1_RECT_F& rect, const D2D1_COLOR_F& color);
    void drawCircle(const D2D1_ELLIPSE& ellipse, const D2D1_COLOR_F& color);
    void drawSprite(ID2D1Bitmap* bitmap, const D2D1_RECT_F& dst, const D2D1_RECT_F* src = nullptr);

    void shutdown();

    static DXRenderer2D* get()
    {
        static DXRenderer2D instance;
        return &instance;
    }

    ID2D1RenderTarget* getRenderTarget() const { return renderTarget; }

private:
    ID2D1Factory* factory = nullptr;
    ID2D1HwndRenderTarget* renderTarget = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;
};