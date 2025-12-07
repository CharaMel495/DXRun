#pragma once
#include <xmmintrin.h>
#include <cmath>
#include <DirectXMath.h>

namespace CaramelEngine
{
    /// <summary>
    /// 2次元ベクトルクラス
    /// </summary>
    class alignas(16) CVector2 
    {
    private:
        __m128 _vec; // x, y, 0, 0

    public:
        // コンストラクタ群
        CVector2() noexcept : _vec(_mm_setzero_ps()) 
        { }

        CVector2(float x, float y) noexcept
            : _vec(_mm_set_ps(0.0f, 0.0f, y, x)) 
        { }

        explicit CVector2(__m128 vec) noexcept : _vec(vec) 
        { }

        float getX() const noexcept { return _vec.m128_f32[0]; }
        float getY() const noexcept { return _vec.m128_f32[1]; }
        void setX(float newX) noexcept { _vec.m128_f32[0] = newX; }
        void setY(float newY) noexcept { _vec.m128_f32[1] = newY; }

        void set(float x, float y) noexcept 
        {
            _vec = _mm_set_ps(0.0f, 0.0f, y, x);
        }

        DirectX::XMVECTOR toXMVECTOR() const noexcept
        {
            return _vec;
        }

        DirectX::XMFLOAT2 toXMFLOAT2() const noexcept
        {
            return { _vec.m128_f32[0], _vec.m128_f32[1] };
        }

        DirectX::XMFLOAT3 toXMFLOAT3() const noexcept
        {
            return { _vec.m128_f32[0], _vec.m128_f32[1], 0 };
        }

        static CVector2 fromXMVECTOR(const DirectX::XMVECTOR& vec) noexcept
        {
            return CVector2(vec);
        }

        // 加減乗除演算
        CVector2 operator+(const CVector2& rhs) const noexcept 
        {
            return CVector2(_mm_add_ps(_vec, rhs._vec));
        }

        CVector2 operator-(const CVector2& rhs) const noexcept 
        {
            return CVector2(_mm_sub_ps(_vec, rhs._vec));
        }

        CVector2 operator*(float scalar) const noexcept 
        {
            return CVector2(_mm_mul_ps(_vec, _mm_set1_ps(scalar)));
        }

        CVector2 operator/(float scalar) const noexcept 
        {
            return CVector2(_mm_div_ps(_vec, _mm_set1_ps(scalar)));
        }

        CVector2 operator-() const noexcept {
            return CVector2(_mm_mul_ps(_vec, _mm_set1_ps(-1.0f)));
        }

        CVector2& operator+=(const CVector2& rhs) noexcept 
        {
            _vec = _mm_add_ps(_vec, rhs._vec);
            return *this;
        }

        CVector2& operator-=(const CVector2& rhs) noexcept 
        {
            _vec = _mm_sub_ps(_vec, rhs._vec);
            return *this;
        }

        CVector2& operator*=(float scalar) noexcept 
        {
            _vec = _mm_mul_ps(_vec, _mm_set1_ps(scalar));
            return *this;
        }

        CVector2& operator/=(float scalar) noexcept 
        {
            _vec = _mm_div_ps(_vec, _mm_set1_ps(scalar));
            return *this;
        }

        // 要素積
        CVector2 operator*(const CVector2& rhs) const noexcept 
        {
            return CVector2(_mm_mul_ps(_vec, rhs._vec));
        }

        // ベクトル長・正規化・内積
        float dot(const CVector2& other) const noexcept 
        {
            __m128 mul = _mm_mul_ps(_vec, other._vec);
            return mul.m128_f32[0] + mul.m128_f32[1];
        }

        float magnitude() const noexcept 
        {
            return std::sqrt(dot(*this));
        }

        float sqrtmagnitude() const noexcept 
        {
            return dot(*this);
        }

        CVector2 normalized() const noexcept 
        {
            float len = magnitude();
            return (*this) * (1.0f / len);
        }

        // 線形補間
        static CVector2 lerp(const CVector2& a, const CVector2& b, float t) 
        {
            return a + (b - a) * t;
        }

        // スプライン補間
        static CVector2 evaluateSpline(const std::vector<CVector2>& points, float t) 
        {
            if (points.size() == 3) 
            {
                float u = 1.0f - t;
                return points[0] * (u * u) + points[1] * (2 * u * t) + points[2] * (t * t);
            }
            else if (points.size() == 4) 
            {
                float u = 1.0f - t;
                return points[0] * (u * u * u)
                    + points[1] * (3 * u * u * t)
                    + points[2] * (3 * u * t * t)
                    + points[3] * (t * t * t);
            }
            else if (points.size() == 2) 
            {
                return lerp(points[0], points[1], t);
            }
            return points.empty() ? CVector2(0, 0) : points[0];
        }
    };
}
