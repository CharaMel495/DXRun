#include "DXLib.h"
#include "MainScene.h"
#include "Player.h"
#include "StageObject.h"

void MainScene::initialize() noexcept
{
	_actors.clear();
	_pendingAdditions.clear();
	_pendingRemovals.clear();

	auto pTransform = std::make_shared<Transform>();
	pTransform->setPosition({ 0.0f, 240.0f, -400.0f });
	pTransform->setRotation(CQuaternion::fromEulerXYZ(0.0f, 180.0f * DEG2RAD, 0.0f));
	pTransform->setScale({ 20.0f, 20.0f, 20.0f });
	auto player = std::make_shared<Player>(pTransform, "Player");
	player->initialize();

	auto stageTransform = std::make_shared<Transform>();
	stageTransform->setPosition({ 0.0f, -100.0f, 0.0f });
	stageTransform->setScale({ 10.0f, 10.0f, 10.0f });
	auto stageObj = std::make_shared<StageObject>(stageTransform, "testStage");
	stageObj->initialize();

	_mainCam = std::make_shared<Camera>();
	_mainCam->setTarget(player);
	_mainCam->initialize();

	addActor(player);
	addActor(stageObj);
}

void MainScene::update() noexcept
{
	//更新処理の過程で登録待ちに追加されたアクタを管理対象に追加
	flushPendingActors();

	for (auto actor : _actors)
		actor->update();

	_mainCam->update(CaramelEngine::Time::getFixedDeltaTime());
	_mainCam->setup();

	flushPendingRemovals();
}

void MainScene::fixedUpdate() noexcept
{
	for (auto actor : _actors)
		actor->fixedUpdate();
}

void MainScene::shutdown() noexcept
{
	_actors.clear();
	_pendingAdditions.clear();
	_pendingRemovals.clear();
}