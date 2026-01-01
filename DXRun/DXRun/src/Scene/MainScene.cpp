#include "DXLib.h"
#include "MainScene.h"
#include "Player.h"
#include "EnemyBase.h"
#include "ItemBase.h"
#include "StageObject.h"
#include "ColliderManager.h"

void MainScene::initialize() noexcept
{
	_actors.clear();
	_pendingAdditions.clear();
	_pendingRemovals.clear();
	
	_colliderManager = std::make_unique<ColliderManager>(&Engine::getInstance().getEventDispathcer());
	_colliderManager->initialize();

	auto pTransform = std::make_shared<Transform>();
	pTransform->setPosition({ 0.0f, 240.0f, -400.0f });
	pTransform->setRotation(CQuaternion::fromEulerXYZ(0.0f, 0.0f, 0.0f));
	pTransform->setScale({ 200.0f, 200.0f, 200.0f });
	auto player = std::make_shared<Player>(pTransform, "Player");
	player->initialize();

	for (int i = 0; i < 10; ++i)
	{
		auto itemTransform = std::make_shared<Transform>();
		itemTransform->setPosition({ 0.0f, 240.0f, (float)(i * 200 + 50) });
		itemTransform->setScale({ 200.0f, 200.0f, 200.0f });
		auto item = std::make_shared<ItemBase>(itemTransform, "item_" + std::to_string(i));
		item->initialize();
		addActor(item);
	}

	auto eTransform = std::make_shared<Transform>();
	eTransform->setPosition({ 200.0f, 240.0f, -400.0f });
	eTransform->setRotation(CQuaternion::fromEulerXYZ(0.0f, 0.0f, 0.0f));
	eTransform->setScale({ 200.0f, 200.0f, 200.0f });
	auto enemy = std::make_shared<EnemyBase>(eTransform, "Enemy");
	enemy->initialize();
	addActor(enemy);

	auto stageTransform = std::make_shared<Transform>();
	stageTransform->setPosition({ 0.0f, 100.0f, 0.0f });
	stageTransform->setScale({ 5000.0f, 5000.0f, 5000.0f });
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
	{
		actor->update();
	
		if (actor->getDestroyFlag())
			removeActor(actor);
	}

	_mainCam->update(CaramelEngine::Time::getFixedDeltaTime());
	_mainCam->setup();

	_colliderManager->update();

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

void MainScene::addActor(std::shared_ptr<Actor> newActor) noexcept
{
	Scene::addActor(newActor);

	auto colObj = newActor->getColliderInterface();

	if (colObj == nullptr)
		return;

	_colliderManager->registerObject(colObj->getCollisionGroupID(), colObj);
}

void MainScene::removeActor(const std::shared_ptr<Actor>& actor) noexcept
{
	Scene::removeActor(actor);

	auto colObj = actor->getColliderInterface();

	if (colObj == nullptr)
		return;

	_colliderManager->unregisterObject(colObj);
}