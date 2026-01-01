#include "DxLib.h"
#include "CaramelEngine.h"
#include "ColliderManager.h"

void ColliderManager::initialize() noexcept
{
    //読みやすさの為、短縮系を用いる
    using CG = eCollisionGroup;

    // ====== メインの戦闘構成 ======
    //プレイヤーvs敵弾
    _collisionPairs.emplace((uint32_t)CG::Player, (uint32_t)CG::EnemyShot);
    //プレイヤー弾vs敵判定
    _collisionPairs.emplace((uint32_t)CG::PlayerShot, (uint32_t)CG::Enemy);
    //プレイヤーvs敵の体当たり判定
    _collisionPairs.emplace((uint32_t)CG::Player, (uint32_t)CG::Enemy);

    // ====== アイテム ======
    //プレイヤーvsアイテム判定
    _collisionPairs.emplace((uint32_t)CG::Player, (uint32_t)CG::Item);
}