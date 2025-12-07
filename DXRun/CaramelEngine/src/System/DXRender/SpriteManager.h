#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Sprite.h"
#include "TextureManager.h"
#include "RectPacker.h"

namespace CaramelEngine
{
    class SpriteManager
    {
    public:
        static SpriteManager& getInstance();

        // Sprite作成
        std::shared_ptr<Sprite> createSprite(
            const std::string& name,
            const std::string& texturePath,
            const CUVRect& uvRect = { 0.f, 0.f, 1.f, 1.f },
            const CVector2& pivot = { 0.5f, 0.5f });

        std::shared_ptr<Sprite> getSprite(const std::string& name) const;

        // 仮想アトラス構築
        void buildVirtualAtlas(int atlasWidth = 2048, int atlasHeight = 2048);

        void clear();

        const std::unordered_map<std::string, std::shared_ptr<Sprite>>& getAllSprites() const noexcept
        {
            return _sprites;
        }


    private:
        SpriteManager() = default;

        // --- アトラス構造 ---
        struct Atlas
        {
            std::string name;
            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
            int width = 0;
            int height = 0;
            std::vector<std::string> spriteNames; // 登録スプライト一覧
        };

        std::unordered_map<std::string, std::shared_ptr<Sprite>> _sprites;
        std::vector<Atlas> _atlases;
    };
}
