#pragma once
#include "CaramelEngine.h"
#include "Camera.h"
#include "ColliderManager.h"

class MainScene final : public Scene
{
public:

	MainScene() noexcept : Scene() {}

	~MainScene() noexcept {}

	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	void shutdown() noexcept override;

	void addActor(std::shared_ptr<Actor> newActor) noexcept override;

	void removeActor(const std::shared_ptr<Actor>& actor) noexcept override;

private:

	std::shared_ptr<Camera> _mainCam;

	std::unique_ptr<ColliderManager> _colliderManager;
};