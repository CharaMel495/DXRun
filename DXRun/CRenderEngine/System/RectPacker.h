#pragma once
#include <vector>
#include <optional>
#include <Windows.h> // RECT構造体用

namespace CaramelEngine
{
    struct CUVRect; // 前方宣言（Sprite.hで定義）

    class RectPacker
    {
    public:
        RectPacker(int width, int height);

        // 矩形を挿入し、成功時はUV座標を返す
        std::optional<CUVRect> insert(int w, int h);

        void clear();
        const Cvector<RECT>& getFreeRects() const { return _freeRects; }

    private:
        int _atlasWidth;
        int _atlasHeight;

        Cvector<RECT> _freeRects;

        RECT findBestFit(int w, int h, bool& success);
        void splitFreeRects(const RECT& placed);
        bool isContained(const RECT& a, const RECT& b);
    };
}
