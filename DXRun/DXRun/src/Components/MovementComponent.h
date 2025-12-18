#pragma once
#include "CaramelEngine.h"

using namespace CaramelEngine;

class MovementComponent final : public Component
{
public:

	MovementComponent() = delete;

	MovementComponent(std::weak_ptr<Actor> owner) noexcept :
		Component(owner, "MovementComponent"),
		_transform(),
		_moveDir(),
		_moveSpeed()
	{
	}

	inline ~MovementComponent() {}

	void initialize() noexcept override;

	void update() noexcept;

	void move() noexcept;

	void setTransform(std::weak_ptr<Transform> transform) noexcept { _transform = transform; }

	void setMoveDir(CVector3 moveDir) noexcept { _moveDir = moveDir; }

	void setMoveSpeed(float moveSpeed) noexcept { _moveSpeed = moveSpeed; }

private:

	std::weak_ptr<Transform> _transform;

	/// <summary>
	/// ˆÚ“®•ûŒü
	/// </summary>
	CVector3 _moveDir;

	/// <summary>
	/// ˆÚ“®‘¬“x
	/// </summary>
	float _moveSpeed;
};