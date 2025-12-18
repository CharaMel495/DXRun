#pragma once
#include <xmmintrin.h>
#include <cmath>
#include <DirectXMath.h>

namespace CaramelEngine
{
    class alignas(16) CQuaternion 
    {
    private:
        __m128 _vec;

    public:
        // === コンストラクタ群 ===
        CQuaternion() noexcept
            : _vec(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f)) 
        { }

        CQuaternion(float x, float y, float z, float w) noexcept
            : _vec(_mm_set_ps(w, z, y, x)) 
        { }

        explicit CQuaternion(__m128 vec) noexcept
            : _vec(vec) 
        { }

        // === アクセッサ ===
        float getX() const noexcept { return _vec.m128_f32[0]; }
        float getY() const noexcept { return _vec.m128_f32[1]; }
        float getZ() const noexcept { return _vec.m128_f32[2]; }
        float getW() const noexcept { return _vec.m128_f32[3]; }

        // === 静的生成メソッド ===
        /// <summary>
        /// 単位四元数を返すメソッド
        /// </summary>
        static CQuaternion identity() 
        {
            return CQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        /// <summary>
        /// 軸と角度から回転を生成するメソッド
        /// </summary>
        /// <param name="axis">軸</param>
        /// <param name="angleRad">何度回転してるか(ラジアン)</param>
        /// <returns>生成された回転</returns>
        static CQuaternion fromAxisAngle(const CVector3& axis, float angleRad) 
        {
            float half = angleRad * 0.5f;
            float s = std::sin(half);
            float c = std::cos(half);
            CVector3 norm = axis.normalized();
            return CQuaternion(norm.getX() * s, norm.getY() * s, norm.getZ() * s, c);
        }

        /// <summary>
        /// オイラー角から回転を生成
        /// </summary>
        /// <param name="pitch">右方向軸の回転</param>
        /// <param name="yaw">上方向軸の回転</param>
        /// <param name="roll">正面方向軸の回転</param>
        /// <returns></returns>
        static CQuaternion fromEulerXYZ(float pitch, float yaw, float roll) 
        {
            using namespace DirectX;
            XMVECTOR q = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
            return CQuaternion(q);
        }

        // === クォータニオン演算 ===
        /// <summary>
        /// 回転の正規化するメソッド
        /// </summary>
        CQuaternion normalized() const 
        {
            float x = getX(), y = getY(), z = getZ(), w = getW();
            float len = std::sqrt(x * x + y * y + z * z + w * w);
            return CQuaternion(x / len, y / len, z / len, w / len);
        }

        /// <summary>
        /// 共役の四元数を生成するメソッド
        /// </summary>
        CQuaternion conjugated() const 
        {
            return CQuaternion(-getX(), -getY(), -getZ(), getW());
        }

        /// <summary>
        /// 他回転との内積を求めるメソッド
        /// </summary>
        float dot(const CQuaternion& other) const 
        {
            return getX() * other.getX() + getY() * other.getY() + getZ() * other.getZ() + getW() * other.getW();
        }

        /// <summary>
        /// 回転の合成(乗算)用の演算子オーバーロード
        /// </summary>
        CQuaternion operator*(const CQuaternion& other) const 
        {
            float x1 = getX(), y1 = getY(), z1 = getZ(), w1 = getW();
            float x2 = other.getX(), y2 = other.getY(), z2 = other.getZ(), w2 = other.getW();
            return CQuaternion(
                w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
                w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
                w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
                w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2
            );
        }

        /// <summary>
        /// 自分が持ってる回転分ベクトルを回転させるメソッド
        /// </summary>
        /// <param name="v">回転させたいベクトル</param>
        /// <returns>回転後のベクトル</returns>
        CVector3 rotateVector(const CVector3& v) const 
        {
            CQuaternion qv(v.getX(), v.getY(), v.getZ(), 0.0f);
            CQuaternion result = (*this) * qv * this->conjugated();
            return CVector3(result.getX(), result.getY(), result.getZ());
        }

        /// <summary>
        /// 球面線形補間を行うメソッド
        /// </summary>
        /// <param name="from">元の回転</param>
        /// <param name="to">目標の回転</param>
        /// <param name="t">何％の地点の回転を返すか</param>
        /// <returns>補間された回転</returns>
        static CQuaternion slerp(const CQuaternion& from, const CQuaternion& to, float t) 
        {
            float dotProd = from.dot(to);
            CQuaternion end = to;
            if (dotProd < 0.0f) 
            {
                end = CQuaternion(-to.getX(), -to.getY(), -to.getZ(), -to.getW());
                dotProd = -dotProd;
            }

            if (dotProd > 0.9995f) 
            {
                return CQuaternion(
                    from.getX() + t * (end.getX() - from.getX()),
                    from.getY() + t * (end.getY() - from.getY()),
                    from.getZ() + t * (end.getZ() - from.getZ()),
                    from.getW() + t * (end.getW() - from.getW())
                ).normalized();
            }
            else 
            {
                float theta = std::acos(dotProd);
                float sinTheta = std::sin(theta);
                float w1 = std::sin((1.0f - t) * theta) / sinTheta;
                float w2 = std::sin(t * theta) / sinTheta;
                return CQuaternion(
                    from.getX() * w1 + end.getX() * w2,
                    from.getY() * w1 + end.getY() * w2,
                    from.getZ() * w1 + end.getZ() * w2,
                    from.getW() * w1 + end.getW() * w2
                );
            }
        }

        // === 変換系 ===
        
        /// <summary>
        /// XMMATRIXに変換するメソッド
        /// </summary>
        DirectX::XMMATRIX toMatrix() const 
        {
            return DirectX::XMMatrixRotationQuaternion(_vec);
        }

        /// <summary>
        /// XMVECTORに変換するメソッド
        /// </summary>
        DirectX::XMVECTOR toXMVECTOR() const 
        {
            return _vec;
        }

        /// <summary>
        /// 自信の回転からオイラー角を生成するメソッド
        /// </summary>
        /// <returns></returns>
        CVector3 toEulerXYZ() const 
        {
            using namespace DirectX;
            XMFLOAT4 q;
            XMStoreFloat4(&q, _vec);

            float pitch;
            float yaw;
            float roll;

            // Yaw (Y)
            yaw = std::atan2(
                2.0f * (q.w * q.y + q.x * q.z),
                1.0f - 2.0f * (q.y * q.y + q.x * q.x)
            );

            // Pitch (X)
            float sinp = 2.0f * (q.w * q.x - q.z * q.y);
            if (std::abs(sinp) >= 1)
                pitch = std::copysign(XM_PIDIV2, sinp);
            else
                pitch = std::asin(sinp);

            // Roll (Z)
            roll = std::atan2(
                2.0f * (q.w * q.z + q.y * q.x),
                1.0f - 2.0f * (q.x * q.x + q.z * q.z)
            );

            return CVector3(pitch, yaw, roll);
        }
    };


}