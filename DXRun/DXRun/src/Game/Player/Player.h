#pragma once
#include "CaramelEngine.h"

class Player final : public Pawn
{
public:

	Player() = delete;

	Player(std::shared_ptr<Transform> transform, CString name) noexcept :
		Pawn(transform, name)
	{}

	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	void destroy() noexcept override;

private:
};