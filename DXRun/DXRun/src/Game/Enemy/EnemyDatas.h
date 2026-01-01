#pragma once
#include "CaramelEngine.h"
#include "ITargetProvider.h"

struct EnemyParam
{
	//ここまでは敵のファクトリメソッドを呼ぶ側が決めれる
	CString name;
	uint32_t spriteID;
	eColliderType colliderType;
	float colliderSize;
	uint32_t hasDamage;
	bool fireEnabled;
	uint32_t useBulletID;
	uint32_t hasScore;
	uint32_t deadEffectID;
	uint32_t dropScoreItemValue;
	uint32_t dropPowerItemValue;


	// ---以下はマネージャ生成時に設定する
	int createID;
	std::shared_ptr<Transform> enemyTransform;
	std::shared_ptr<ITargetProvider> target;
	CVector3 startDir;
	float moveSpeed;
};

/// <summary>
/// Jsonから受け取ったデータを格納する構造体
/// </summary>
struct EnemyMasterDataReciever
{
	int32_t id = -1;
	CString name = "";
	uint32_t spriteId = 0;
	uint32_t colliderType = 0;
	float colliderSize = 0;
	uint32_t hasDamage = 0;
	uint32_t moveType = 0;
	float moveSpeed = 0.0f;
	bool fireEnabled = false;
	uint32_t fireType = 0;
	uint32_t bulletTableId = 0;
	uint32_t score = 0;
	uint32_t deadEffectType = 0;
	uint32_t dropScoreItemValue = 0;
	uint32_t dropPowerItemValue = 0;
};