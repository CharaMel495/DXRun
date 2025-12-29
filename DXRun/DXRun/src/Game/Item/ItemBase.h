#pragma once
#include "CaramelEngine.h"

class ItemBase : public Actor
{
public:

	ItemBase(std::shared_ptr<Transform> transform, CString name) noexcept :
		Actor(transform, name)
	{
	}

	// --- MonoBehaviour ---
	virtual void initialize() noexcept override;

	virtual void update() noexcept override;

	virtual void fixedUpdate() noexcept override;

	// --- Actor ---
	void destroy() noexcept override;

private:
};