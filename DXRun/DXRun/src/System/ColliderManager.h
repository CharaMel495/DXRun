#pragma once

using namespace CaramelEngine;

#pragma once
#include <cstdint>

enum class eCollisionGroup : uint32_t
{
    None = 0,
    Player,            // 自機の本体
    PlayerShot,        // 通常ショット
    Enemy,             // 敵本体（被弾対象）
    EnemyShot,         // 敵の弾
    Item,              // アイテム系

};

/// <summary>
/// このゲームの衝突関係に責任を持つクラス
/// </summary>
class ColliderManager final : public ColliderManagerBase
{
public:

    inline ColliderManager() noexcept = delete;

    ColliderManager(EventDispatcher* dispatcher) noexcept :
        ColliderManagerBase(dispatcher),
        _collisionPairs()
    {}

    ~ColliderManager() override = default;

    void initialize() noexcept override;

private:

    //衝突させるグループのペアを返す（固定でOK）
    const std::unordered_set<std::pair<uint32_t, uint32_t>, PairHash>& getCollisionPairs() const override 
    { return _collisionPairs; }

    //衝突関係があるものを記録した変数
    std::unordered_set<std::pair<uint32_t, uint32_t>, PairHash> _collisionPairs;
};
