#pragma once
#include "CaramelEngine.h"

class StageObject : public Actor
{
public:

	StageObject(std::shared_ptr<Transform> transform, CString name) noexcept :
		Actor(transform, name),
		_itemNum(),
		_itemPopInterval(2.0f),
		_itemTimer(0.0f)
	{
	}

	// --- MonoBehaviour ---
	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	// --- Actor ---
	void destroy() noexcept override;

	// --- StageObject ---
	void popItem();
	void popEnemy();

private:

	int _itemNum;

	const float _itemPopInterval;

	float _itemTimer;
};