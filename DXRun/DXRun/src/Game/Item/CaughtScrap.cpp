#include "CaughtScrap.h"
#include "MovementComponent.h"

void CaughtScrap::fixedUpdate() noexcept
{
	ItemBase::fixedUpdate();

	if (!_isThrowed || isInScreen(Engine::getInstance().getSceneManager().getCurrentScene()->getMainCamera()))
		return;

	_remainTime -= CaramelEngine::Time::getFixedDeltaTime();

	if (_remainTime > 0)
		return;

	destroy();
}

// がらくた取得メソッド
void CaughtScrap::addScrap(int addValue) 
{ 
	auto before = _caughtValue;
	_caughtValue += addValue;
	if (_caughtValue > MAXSCRAPVALUE)
	{
		_caughtValue = MAXSCRAPVALUE;
		if (before == _caughtValue)
			return;

		addValue = _caughtValue - before;
	}

	auto scale = _transform->getScale();
	scale.setX(scale.getX() + addValue * _scaleMulByValue);
	scale.setY(scale.getY() + addValue * _scaleMulByValue);
	scale.setZ(scale.getZ() + addValue * _scaleMulByValue);
	_transform->setScale(scale);
}

// がらくた投擲メソッド
void CaughtScrap::throwScrap(CVector3 throwDir)
{
	float t = INVERSE_LERP_CLAMP(0.0f, MAXSCRAPVALUE, _caughtValue);
	_throwSpeed = std::lerp(_minSpeed, _maxSpeed, 1 - t);

	// 移動用コンポーネントを追加
	auto mover = addComponent<MovementComponent>();
	mover->setTransform(_transform);
	mover->setMoveDir(throwDir);
	mover->setMoveSpeed(_throwSpeed);

	// プレイヤーの親になってるハズなので、離脱
	_transform->setParent(nullptr);

	_isThrowed = true;
}