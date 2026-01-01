#pragma once
#include "CaramelEngine.h"
#include "ColliderManager.h"
#include "ITargetProvider.h"

class EnemyParam
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

class EnemyBase : public Actor, public ICollisionObject
{
public:

	EnemyBase(std::shared_ptr<Transform> transform, CString name) noexcept :
		Actor(transform, name),
		ICollisionObject(_actorName),
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

	//衝突イベント３種
	void onCollisionEnter(void* data) noexcept override;
	void onCollisionStay(void* data) noexcept override;
	void onCollisionExit(void* data) noexcept override;

private:

	ICollider* _col;
};