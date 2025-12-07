#include "pch.h"

bool CaramelEngine::Actor::isInScreen() const noexcept
{
	const auto& pos = _transform->getPosition();
	const auto screenSize = Engine::getInstance().getScreenSize(); // {width, height}

	return (pos.getX() >= 0 && pos.getX() <= screenSize.getX() &&
		pos.getY() >= 0 && pos.getY() <= screenSize.getY());
}