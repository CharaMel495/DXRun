#pragma once

namespace CaramelEngine
{
    __declspec(align(16))
        class Light
    {
    private:
        DirectX::XMVECTOR _directional; // 平行光源の方向ベクトル
        DirectX::XMVECTOR _position;    // 点光源の位置
        DirectX::XMFLOAT4 _ambient;     // 環境光
        DirectX::XMFLOAT4 _diffuse;     // 拡散光
        DirectX::XMFLOAT4 _speqular;    // 鏡面反射光
        DirectX::XMFLOAT4 _attenuation; // 減衰率

    public:
        Light();
        ~Light() = default;

        void SetDirectionalLight(const DirectX::XMVECTOR);
        void SetLightPosition(const DirectX::XMVECTOR);
        void SetAmbientLight(const DirectX::XMFLOAT4);
        void SetDiffuseLight(const DirectX::XMFLOAT4);
        void SetSpeqularLight(const DirectX::XMFLOAT4);
        void SetAttenuation(const DirectX::XMFLOAT4);

        DirectX::XMVECTOR GetDirectionalLight() const { return _directional; }
        DirectX::XMVECTOR GetLightPosition() const { return _position; }
        DirectX::XMFLOAT4 GetAmbinetLight() const { return _ambient; }
        DirectX::XMFLOAT4 GetDiffuseLight() const { return _diffuse; }
        DirectX::XMFLOAT4 GetSpeqularLight() const { return _speqular; }
        DirectX::XMFLOAT4 GetAttenuation() const { return _attenuation; }

        void* operator new(size_t size) {
            return _mm_malloc(size, alignof(Light));
        }
        void operator delete(void* p) {
            return _mm_free(p);
        }
    };
}