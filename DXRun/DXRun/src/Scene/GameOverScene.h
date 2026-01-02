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

	void addActor(std::shared_ptr<Actor> newActor) noexcept override;

	void removeActor(const std::shared_ptr<Actor>& actor) noexcept override;

private:
};