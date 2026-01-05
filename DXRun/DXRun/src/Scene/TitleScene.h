#pragma once
#include "CaramelEngine.h"
#include "Camera.h"
#include "ColliderManager.h"
#include "EnemyManager.h"

class TitleScene final : public Scene
{
public:

	TitleScene() noexcept : Scene() {}

	~TitleScene() noexcept {}

	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	void shutdown() noexcept override;

	void onPressedKey(void* data) noexcept;

private:
};