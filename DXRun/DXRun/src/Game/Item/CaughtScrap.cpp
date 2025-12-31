#include "CaughtScrap.h"
#include "MovementComponent.h"


// がらくた取得メソッド
void CaughtScrap::addScrap(int addValue) 
{ 
	_caughtValue = (std::min)(_caughtValue + addValue, MAXSCRAPVALUE); 

	auto scale = _transform->getScale();
	scale.setX(scale.getX() + addValue * _scaleMulByValue);
	scale.setY(scale.getY() + addValue * _scaleMulByValue);
	scale.setZ(scale.getZ() + addValue * _scaleMulByValue);
	_transform->setScale(scale);
}

// がらくた投擲メソッド
void CaughtScrap::throwScrap(CVector3 throwDir)
{
	// 移動用コンポーネントを追加
	auto mover = addComponent<MovementComponent>();
	mover->setTransform(_transform);
	mover->setMoveDir(throwDir);
	mover->setMoveSpeed(_throwSpeed);

	// プレイヤーの親になってるハズなので、離脱
	_transform->setParent(nullptr);
}