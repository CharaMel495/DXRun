#pragma once
#include "CaramelEngine.h"
#include "ItemBase.h"

constexpr int MAXSCRAPVALUE = 15;

class CaughtScrap : public ItemBase
{
public:

	CaughtScrap(std::shared_ptr<Transform> transform, CString name) :
		ItemBase(transform, name),
		_caughtValue(0),
		_scaleMulByValue(0.1),
		_throwSpeed(10.0f)
	{}

	// --- ItemBase ---
	// CollisionGroupを返す（どのグループに属するか）
	virtual std::uint32_t getCollisionGroupID() const { return (std::uint32_t)eCollisionGroup::PlayerShot; }

	// がらくた取得メソッド
	void addScrap(int addValue = 1);

	// がらくた投擲メソッド
	void throwScrap(CVector3 throwDir);

private:

	// 集めたスクラップの量
	int _caughtValue;

	// ガラクタ１つあたりのスケール増加量
	float _scaleMulByValue;

	float _throwSpeed;
};