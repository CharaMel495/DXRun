#pragma once
#include "CaramelEngine.h"
#include "ITargetProvider.h"
#include "ColliderManager.h"

class Player final : public Pawn, public ICollisionObject, public ITargetProvider
{
public:

	Player() = delete;

	Player(std::shared_ptr<Transform> transform, CString name) noexcept :
		Pawn(transform, name),
		ICollisionObject(_actorName),
		ITargetProvider(),
		_currentDir(),
		_moveSpeed(10.0f),
		_rotSpeed(1.f)
	{}

	// --- MonoBehaviour ---
	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

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
	std::uint32_t getCollisionGroupID() const { return (std::uint32_t)eCollisionGroup::Player; }

	// 自身のポインタを返す
	void* getOwnerPointer() const override
	{
		return reinterpret_cast<void*>(const_cast<Player*>(this));
	}

	//衝突イベント３種
	void onCollisionEnter(void* data) noexcept override;
	void onCollisionStay(void* data) noexcept override;
	void onCollisionExit(void* data) noexcept override;

	// --- ITargetProvider ---
	CVector3 getTargetPosition() const noexcept override { return _transform->getPosition(); }

	// --- Playerの実装 ---

	//キー入力イベント(押された瞬間)
	void onPressedKey(void* key) noexcept;

	//キー入力イベント(継続的に押されている)
	void onHeldKey(void* key) noexcept;

	//キー入力イベント(離された瞬間)
	void onReleasedKey(void* key) noexcept;

	void onStickActiveL(void* stick) noexcept;
	void onStickReleaseL(void* stick) noexcept;
	void onStickActiveR(void* stick) noexcept;
	void onStickReleaseR(void* stick) noexcept;

	
private:


	/// <summary>
	/// 移動速度
	/// </summary>
	float _moveSpeed;

	/// <summary>
	/// 回転速度
	/// </summary>
	float _rotSpeed;

	/// <summary>
	/// 現在の向き
	/// </summary>
	CVector3 _currentDir;

	ICollider* _col;
};