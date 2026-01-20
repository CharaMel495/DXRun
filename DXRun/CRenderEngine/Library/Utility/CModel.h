#pragma once
#include <vector>
#include <string>
#include <wrl/client.h>
#include <d3d11.h>

namespace CaramelEngine
{
    class GraphicsDevice;

    class CModel
    {
    public:
        struct CMesh
        {
            Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
            Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
            uint32_t indexCount = 0;
        };

        CModel() = default;
        ~CModel() = default;

        // ファイル or ディレクトリ指定でロード
        bool loadFromFile(
            GraphicsDevice& gfx,
            const std::string& path
        );

        bool loadFromDirectory(
            GraphicsDevice& gfx,
            const std::string& directory,
            const std::string& filename
        );

        const std::vector<CMesh>& getMeshes() const noexcept
        {
            return _meshes;
        }

    private:
        std::vector<CMesh> _meshes;

        // 内部処理（cppに隠す）
        bool processAssimpScene(
            GraphicsDevice& gfx,
            const struct aiScene* scene
        );

        void processAssimpMesh(
            GraphicsDevice& gfx,
            const struct aiMesh* mesh
        );
    };
}
