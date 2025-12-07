#pragma once

namespace CaramelEngine
{
    __declspec(align(16))
        class Model
    {
    private:
        std::unique_ptr<MeshRenderer> mesh;

        //DirectX::XMMATRIX _world;

        std::shared_ptr<Transform> _transform;

    public:
        Model(std::shared_ptr<Transform> transform);
        ~Model() = default;

        HRESULT CreateModel(LPCWSTR);
        void MoveModel(_In_opt_ std::function<DirectX::XMMATRIX()> customFunction = nullptr);
        void RenderModel(
            _In_opt_ std::function<void(DirectX::CXMMATRIX, const SDKMESH_MATERIAL*)> customFunction = nullptr,
            const UINT textureSlot = 0);

        DirectX::XMMATRIX GetWorld() const { return _transform->getWorldMatrix(); }

        std::shared_ptr<Transform> getTransform() { return _transform; }

        void* operator new(size_t size) 
        {
            return _mm_malloc(size, alignof(Model));
        }
        void operator delete(void* p) 
        {
            return _mm_free(p);
        }
    };
}