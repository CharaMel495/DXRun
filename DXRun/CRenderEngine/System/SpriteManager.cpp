#include "pch.h"
#include "SpriteManager.h"

using Microsoft::WRL::ComPtr;

namespace CaramelEngine
{
    SpriteManager& SpriteManager::getInstance()
    {
        static SpriteManager instance;
        return instance;
    }

    std::shared_ptr<Sprite> SpriteManager::createSprite(
        const std::string& name,
        const std::string& texturePath,
        const CUVRect& uvRect,
        const CVector2& pivot)
    {
        auto& texMgr = TextureManager::getInstance();
        auto info = texMgr.loadTexture(texturePath);
        if (!info)
            return nullptr;

        auto sprite = std::make_shared<Sprite>(name, info->srv.Get(), uvRect, pivot);
        sprite->setAtlasName(texturePath); // 初期状態は個別テクスチャ名

        _sprites[name] = sprite;
        return sprite;
    }

    std::shared_ptr<Sprite> SpriteManager::getSprite(const std::string& name) const
    {
        auto it = _sprites.find(name);
        return (it != _sprites.end()) ? it->second : nullptr;
    }

    void SpriteManager::clear()
    {
        _sprites.clear();
        _atlases.clear();
    }

    void SpriteManager::buildVirtualAtlas(int atlasWidth, int atlasHeight)
    {
        auto& texMgr = TextureManager::getInstance();
        ID3D11Device* device = texMgr.getDevice();
        ID3D11DeviceContext* context = texMgr.getContext();

        RectPacker packer(atlasWidth, atlasHeight);

        struct PendingTex
        {
            std::string spriteName;
            const TextureManager::TextureInfo* texInfo;
            CUVRect uv;
        };

        std::vector<PendingTex> packedList;
        Atlas currentAtlas;
        currentAtlas.name = "Atlas_0";
        currentAtlas.width = atlasWidth;
        currentAtlas.height = atlasHeight;

        for (auto& [name, sprite] : _sprites)
        {
            auto info = texMgr.getTextureInfo(sprite->getAtlasName());
            if (!info)
                continue;

            auto regionUV = packer.insert(info->width, info->height);
            if (!regionUV)
            {
                // 現アトラスに入りきらない場合 → 新しいアトラスを作成
                _atlases.push_back(std::move(currentAtlas));
                currentAtlas = Atlas();
                currentAtlas.name = "Atlas_" + std::to_string(_atlases.size());
                currentAtlas.width = atlasWidth;
                currentAtlas.height = atlasHeight;
                packer.clear();

                regionUV = packer.insert(info->width, info->height);
            }

            packedList.push_back({ name, info, *regionUV });
            currentAtlas.spriteNames.push_back(name);
        }

        // 最後のアトラスを追加
        _atlases.push_back(std::move(currentAtlas));

        // --- DirectXで実際にアトラステクスチャを構築 ---
        for (size_t atlasIdx = 0; atlasIdx < _atlases.size(); ++atlasIdx)
        {
            auto& atlas = _atlases[atlasIdx];

            D3D11_TEXTURE2D_DESC desc = {};
            desc.Width = atlasWidth;
            desc.Height = atlasHeight;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.Usage = D3D11_USAGE_DEFAULT;

            ComPtr<ID3D11Texture2D> atlasTex;
            HRESULT hr = device->CreateTexture2D(&desc, nullptr, &atlasTex);
            if (FAILED(hr)) continue;

            // 各テクスチャを貼り付け
            for (auto& entry : packedList)
            {
                if (std::find(atlas.spriteNames.begin(), atlas.spriteNames.end(), entry.spriteName)
                    == atlas.spriteNames.end())
                    continue;

                const auto& info = *entry.texInfo;
                auto& uv = entry.uv;

                D3D11_BOX srcBox = { 0, 0, 0, info.width, info.height, 1 };
                int destX = static_cast<int>(uv.u * atlasWidth);
                int destY = static_cast<int>(uv.v * atlasHeight);

                context->CopySubresourceRegion(
                    atlasTex.Get(),
                    0,
                    destX,
                    destY,
                    0,
                    info.texture.Get(),
                    0,
                    &srcBox);
            }

            // SRV作成
            ComPtr<ID3D11ShaderResourceView> srv;
            hr = device->CreateShaderResourceView(atlasTex.Get(), nullptr, &srv);
            if (FAILED(hr)) continue;

            atlas.texture = atlasTex;
            atlas.srv = srv;

            // --- Sprite側を更新 ---
            for (auto& entry : packedList)
            {
                if (std::find(atlas.spriteNames.begin(), atlas.spriteNames.end(), entry.spriteName)
                    == atlas.spriteNames.end())
                    continue;

                auto sprite = _sprites[entry.spriteName];
                sprite->setSRV(srv.Get());
                sprite->setUVRect(entry.uv);
                sprite->setAtlasName(atlas.name);
            }
        }
    }
}

