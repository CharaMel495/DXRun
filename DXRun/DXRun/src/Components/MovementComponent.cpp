#include "MovementComponent.h"

void MovementComponent::initialize() noexcept
{

}

void MovementComponent::update() noexcept
{
	move();
}

void MovementComponent::move() noexcept
{
	if (auto transform = _transform.lock())
	{
		auto pos = transform->getPosition();
		pos += _moveDir * _moveSpeed;
		transform->setPosition(pos);
	}
}
