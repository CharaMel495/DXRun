#pragma once
#include "CaramelEngine.h"
#include "Actor/CCharacter.h"

class GroundCheckRay
{
public:
	GroundCheckRay(eColliderType collisionType, CCharacter* character);

	~GroundCheckRay();

private:
	//std::list<CRay*> _rays;

	eColliderType CollisionType;
	CCharacter* Character;

	void SetRayPos();
};

