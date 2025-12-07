#pragma once
#include <xmmintrin.h>
#include <cmath>
#include <iostream>
#include <DirectXMath.h>

namespace CaramelEngine
{
    /// <summary>
    /// SIMD対応3次元ベクトルクラス
    /// </summary>
    struct CVector3
    {
    private:
        __m128 _vec;

    public:

        //デフォルトコンストラクタ
        CVector3() noexcept : _vec(_mm_setzero_ps()) {}

        //コンストラクタ
        CVector3(float x, float y, float z) noexcept
            : _vec(_mm_set_ps(0.0f, z, y, x)) 
        { }

        //コンストラクタ（__m128引数）
        CVector3(__m128 vec) noexcept
            : _vec(vec) 
        { }

        //コピーコンストラクタ
        CVector3(const CVector3& other) noexcept : _vec(other._vec) {}
        
        // アクセッサ
        float getX() const noexcept { return _vec.m128_f32[0]; }
        float getY() const noexcept { return _vec.m128_f32[1]; }
        virtual float getZ() const noexcept { return _vec.m128_f32[2]; }
        void set(float newX, float newY = 0.0f, float newZ = 0.0f) noexcept {
            _vec = _mm_set_ps(0.0f, newZ, newY, newX);
        }
        void setX(float newX) noexcept { _vec.m128_f32[0] = newX; }
        void setY(float newY) noexcept { _vec.m128_f32[1] = newY; }
        virtual void setZ(float newZ) noexcept { _vec.m128_f32[2] = newZ; }

        DirectX::XMVECTOR toXMVECTOR() const noexcept 
        {
            return _vec;
        }

        static CVector3 fromXMVECTOR(const DirectX::XMVECTOR& vec) noexcept 
        {
            return CVector3(vec);
        }

        //代入演算子オーバーロード
        CVector3& operator=(const CVector3& other) noexcept
        {
            _vec = other._vec;
            return *this;
        }

        //SIMD加算代入
        CVector3& operator+=(const CVector3& other) noexcept
        {
            _vec = _mm_add_ps(_vec, other._vec);
            return *this;
        }

        //SIMD減算代入
        CVector3& operator-=(const CVector3& other) noexcept
        {
            _vec = _mm_sub_ps(_vec, other._vec);
            return *this;
        }

        //SIMDスカラー乗算代入
        CVector3& operator*=(float scalar) noexcept
        {
            __m128 s = _mm_set1_ps(scalar);
            _vec = _mm_mul_ps(_vec, s);
            return *this;
        }

        //SIMDスカラー除算代入
        CVector3& operator/=(float scalar) noexcept
        {
            __m128 s = _mm_set1_ps(scalar);
            _vec = _mm_div_ps(_vec, s);
            return *this;
        }

        //SIMD符号反転
        CVector3 operator-() const noexcept
        {
            __m128 neg = _mm_set1_ps(-1.0f);
            return CVector3(_mm_mul_ps(_vec, neg));
        }

        //SIMD加算
        CVector3 operator+(const CVector3& other) const noexcept
        {
            return CVector3(_mm_add_ps(_vec, other._vec));
        }

        //SIMD減算
        CVector3 operator-(const CVector3& other) const noexcept
        {
            return CVector3(_mm_sub_ps(_vec, other._vec));
        }

        //スカラー乗算
        CVector3 operator*(float scalar) const noexcept
        {
            __m128 s = _mm_set1_ps(scalar);
            return CVector3(_mm_mul_ps(_vec, s));
        }

        CVector3 operator/(float scalar) const noexcept
        {
            __m128 s = _mm_set1_ps(scalar);
            return CVector3(_mm_div_ps(_vec, s));
        }

        //各要素の乗算（アダマール積）
        CVector3 operator*(const CVector3& other) const noexcept
        {
            return CVector3(_mm_mul_ps(_vec, other._vec));
        }

        //内積
        float dot(const CVector3& other) const noexcept 
        {
            return getX() * other.getX() + getY() * other.getY() + getZ() * other.getZ();
        }

        //外積 (SIMD最適化)
        CVector3 cross(const CVector3& other) const noexcept
        {
            __m128 a = _vec;
            __m128 b = other._vec;
            __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 c = _mm_sub_ps(_mm_mul_ps(a, b_yzx), _mm_mul_ps(a_yzx, b));
            __m128 result = _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));
            return CVector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
        }

        //正規化
        CVector3 normalized() const noexcept
        {
            float length = magnitude();
            return (*this) * (1.0f / length);
        }

        //長さ
        float magnitude() const noexcept
        {
            return std::sqrt(dot(*this));
        }

        //長さ
        float sqrtmagnitude() const noexcept
        {
            return dot(*this);
        }

        static CVector3 rotateVector(const CVector3& vec, const CVector3& axis, float angleRad)
        {
            // ロドリゲスの回転公式
            CVector3 k = axis.normalized();
            return vec * std::cos(angleRad)
                + k.cross(vec) * std::sin(angleRad)
                + k * (k.dot(vec)) * (1 - std::cos(angleRad));
        }

        static CVector3 lerp(const CVector3& a, const CVector3& b, float t)
        {
            return a + (b - a) * t;
        }
    };
}