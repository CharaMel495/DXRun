#pragma once
#include <string>
#include <unordered_map>
#include "JsonReader.h"
#include "EnemyDatas.h"

/// <summary>
/// Jsonから
/// </summary>
class EnemyTypeMaster
{
public:
    /// <summary>
    /// JSONファイルからマスタデータを読み込む
    /// </summary>
    bool loadFromJson();

    /// <summary>
    /// 名前（enemyType）からデータを取得
    /// </summary>
    const EnemyParam getEnemyDataById(const uint32_t& iD, CVector3 pos, CVector3 startDir, std::shared_ptr<ITargetProvider> target = nullptr) const;

private:
    std::unordered_map<uint32_t, EnemyMasterDataReciever> _table;
};

