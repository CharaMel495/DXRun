#pragma once
#include "CaramelEngine.h"
#include "Camera.h"
#include "ColliderManager.h"
#include "EnemyManager.h"

class GameOverScene final : public Scene
{
public:

	GameOverScene() noexcept : Scene() {}

	~GameOverScene() noexcept {}

	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	void shutdown() noexcept override;

	void onPressedKey(void* data) noexcept;

private:
};