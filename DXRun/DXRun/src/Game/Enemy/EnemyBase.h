#pragma once
#include "CaramelEngine.h"
#include "EnemyDatas.h"
#include "ColliderManager.h"
#include "ITargetProvider.h"
#include "EventDefinition.h"

class EnemyBase : public Actor, public ICollisionObject
{
public:

	EnemyBase() noexcept = delete;

	EnemyBase(EnemyParam param) noexcept :
		Actor(param.enemyTransform, param.name),
		ICollisionObject(_actorName),
		_param(param),
		_col(nullptr)
	{
	}

	// --- MonoBehaviour ---
	virtual void initialize() noexcept override;

	virtual void update() noexcept override;

	virtual void fixedUpdate() noexcept override;

	// --- Actor ---
	void destroy() noexcept override;

	std::shared_ptr<ICollisionObject> getColliderInterface() noexcept override
	{
		return std::shared_ptr<ICollisionObject>(
			shared_from_this(),
			static_cast<ICollisionObject*>(this)
		);
	}

	// --- ICollisionObject ---
	// 衝突判定に使うコライダー
	ICollider* getCollider() const override
	{
		return _col;
	}

	// CollisionGroupを返す（どのグループに属するか）
	virtual std::uint32_t getCollisionGroupID() const { return (std::uint32_t)eCollisionGroup::Enemy; }

	// 自身のポインタを返す
	void* getOwnerPointer() const override
	{
		return reinterpret_cast<void*>(const_cast<EnemyBase*>(this));
	}

	// 衝突イベント３種
	void onCollisionEnter(void* data) noexcept override;
	void onCollisionStay(void* data) noexcept override;
	void onCollisionExit(void* data) noexcept override;

	// Enemyの実装

	// 破棄待ちか
	bool isDestroyWaiting() noexcept { return _currentHP <= 0; }

	// イベント関係
	void createEvents() noexcept;
	DamageEvent getDamageEventData() noexcept { return _damageEvent; }

private:

	EnemyParam _param;
	ICollider* _col;
	int _currentHP;
	DamageEvent _damageEvent;
};