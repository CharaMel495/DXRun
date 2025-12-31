#pragma once
#include "CaramelEngine.h"
#include "ColliderManager.h"

class ItemBase : public Actor, public ICollisionObject
{
public:

	ItemBase(std::shared_ptr<Transform> transform, CString name) noexcept :
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
	virtual std::uint32_t getCollisionGroupID() const { return (std::uint32_t)eCollisionGroup::Item; }

	// 自身のポインタを返す
	void* getOwnerPointer() const override
	{
		return reinterpret_cast<void*>(const_cast<ItemBase*>(this));
	}

	//衝突イベント３種
	void onCollisionEnter(void* data) noexcept override;
	void onCollisionStay(void* data) noexcept override;
	void onCollisionExit(void* data) noexcept override;

private:

	ICollider* _col;
};