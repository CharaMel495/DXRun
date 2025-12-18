#pragma once
#include "CaramelEngine.h"

class StageObject : public Actor
{
public:

	StageObject(std::shared_ptr<Transform> transform, CString name) noexcept :
		Actor(transform, name)
	{
	}

	// --- MonoBehaviour ---
	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	// --- Actor ---
	void destroy() noexcept override;

private:
};