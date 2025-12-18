#pragma once
#include "CaramelEngine.h"
#include "Camera.h"

class MainScene final : public Scene
{
public:

	MainScene() noexcept : Scene() {}

	~MainScene() noexcept {}

	void initialize() noexcept override;

	void update() noexcept override;

	void fixedUpdate() noexcept override;

	void shutdown() noexcept override;

private:

	std::shared_ptr<Camera> _mainCam;

};