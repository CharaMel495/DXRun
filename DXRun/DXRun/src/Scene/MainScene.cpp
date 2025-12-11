#include "DXLib.h"
#include "MainScene.h"
#include "Player.h"

void MainScene::initialize() noexcept
{
	_actors.clear();
	_pendingAdditions.clear();
	_pendingRemovals.clear();

	auto pTransform = std::make_shared<Transform>();
	pTransform->setPosition({ 360.0f, 240.0f, 300.0f });
	pTransform->setRotation(CQuaternion::fromEulerXYZ(0.0f, 90.0f * DX_PI_F / 180.0f, 0.0f));
	pTransform->setScale({ 20.0f, 20.0f, 20.0f });
	auto player = std::make_shared<Player>(pTransform, "Player");
	player->initialize();

	addActor(player);
}

void MainScene::update() noexcept
{
	//更新処理の過程で登録待ちに追加されたアクタを管理対象に追加
	flushPendingActors();

	for (auto actor : _actors)
		actor->update();

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