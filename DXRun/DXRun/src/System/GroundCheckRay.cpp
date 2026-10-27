#include "GroundCheckRay.h"


GroundCheckRay::GroundCheckRay(eColliderType collisionType,CCharacter* character)
	: CollisionType(collisionType),Character(character)
{
	SetRayPos();
}

GroundCheckRay::~GroundCheckRay()
{
	for (auto& ray : _rays)
	{
		delete ray;
	}
	_rays.clear();
}

void GroundCheckRay::SetRayPos()
{
	auto ray = std::make_shared<CaramelEngine::CRay>(Character->weak_from_this());


}
