#include "pch.h"
#include "RectPacker.h"
#include "Sprite.h"   // CUVRectを使うために追加

namespace CaramelEngine
{
    RectPacker::RectPacker(int width, int height)
        : _atlasWidth(width), _atlasHeight(height)
    {
        clear();
    }

    void RectPacker::clear()
    {
        _freeRects.clear();
        RECT initial = { 0, 0, _atlasWidth, _atlasHeight };
        _freeRects.push_back(initial);
    }

    std::optional<CUVRect> RectPacker::insert(int w, int h)
    {
        bool success = false;
        RECT best = findBestFit(w, h, success);
        if (!success)
            return std::nullopt;

        // 配置矩形を反映して空きリストを分割
        splitFreeRects(best);

        // 分割後の空き矩形同士で内包関係を整理
        for (size_t i = 0; i < _freeRects.size(); ++i)
        {
            for (size_t j = i + 1; j < _freeRects.size(); )
            {
                if (isContained(_freeRects[i], _freeRects[j]))
                {
                    _freeRects.erase(_freeRects.begin() + j);
                }
                else if (isContained(_freeRects[j], _freeRects[i]))
                {
                    _freeRects.erase(_freeRects.begin() + i);
                    --i;
                    break;
                }
                else
                {
                    ++j;
                }
            }
        }

        // ピクセル → UV に変換して返す
        CUVRect uv;
        uv.u = static_cast<float>(best.left) / _atlasWidth;
        uv.v = static_cast<float>(best.top) / _atlasHeight;
        uv.width = static_cast<float>(best.right - best.left) / _atlasWidth;
        uv.height = static_cast<float>(best.bottom - best.top) / _atlasHeight;

        return uv;
    }

    RECT RectPacker::findBestFit(int w, int h, bool& success)
    {
        success = false;
        RECT bestRect{};
        int bestShortSide = INT_MAX;

        for (auto& free : _freeRects)
        {
            int freeW = free.right - free.left;
            int freeH = free.bottom - free.top;

            if (freeW >= w && freeH >= h)
            {
                int leftoverH = abs(freeH - h);
                int leftoverW = abs(freeW - w);
                int shortSide = std::min(leftoverW, leftoverH);

                if (shortSide < bestShortSide)
                {
                    bestRect = { free.left, free.top, free.left + w, free.top + h };
                    bestShortSide = shortSide;
                    success = true;
                }
            }
        }
        return bestRect;
    }

    void RectPacker::splitFreeRects(const RECT& placed)
    {
        Cvector<RECT> newRects;

        for (auto& free : _freeRects)
        {
            // 衝突していなければそのまま
            if (placed.right <= free.left || placed.left >= free.right ||
                placed.bottom <= free.top || placed.top >= free.bottom)
            {
                newRects.push_back(free);
                continue;
            }

            // 左
            if (placed.left > free.left)
                newRects.push_back({ free.left, free.top, placed.left, free.bottom });
            // 右
            if (placed.right < free.right)
                newRects.push_back({ placed.right, free.top, free.right, free.bottom });
            // 上
            if (placed.top > free.top)
                newRects.push_back({ free.left, free.top, free.right, placed.top });
            // 下
            if (placed.bottom < free.bottom)
                newRects.push_back({ free.left, placed.bottom, free.right, free.bottom });
        }

        _freeRects = std::move(newRects);
    }

    bool RectPacker::isContained(const RECT& a, const RECT& b)
    {
        return (a.left >= b.left &&
            a.top >= b.top &&
            a.right <= b.right &&
            a.bottom <= b.bottom);
    }
}
