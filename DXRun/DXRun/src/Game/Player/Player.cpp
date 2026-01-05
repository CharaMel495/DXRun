#include "Player.h"
#include "DXLibModelRender.h"
#include "MoveMentComponent.h"
#include "Scrap.h"
#include "CaughtScrap.h"
#include <cmath>

void Player::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("temp//Models//Test//cube.mv1");
	modelRender->initialize();

	//移動用コンポーネントを取得
	auto mover = addComponent<MovementComponent>();
	//トランスフォームと移動関係を設定
	mover->setTransform(this->_transform);
	mover->setMoveSpeed(this->_moveSpeed);
	mover->setMoveDir(_currentDir);

	auto cube = addComponent<CCube>();
	cube->setTransform(_transform);
	_col = cube;

	_remainInvincibleTime = 0.0f;

	//イベントの登録
	Engine::getInstance().getEventDispathcer().subscribeEvent("onPressedKey", [this](void* data) { onPressedKey(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onHeldKey", [this](void* data) { onHeldKey(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onReleasedKey", [this](void* data) { onReleasedKey(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onStickActiveL", [this](void* data) { onStickActiveL(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onStickReleaseL", [this](void* data) { onStickReleaseL(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onStickActiveR", [this](void* data) { onStickActiveR(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("onStickReleaseR", [this](void* data) { onStickReleaseR(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionEnterEventKey(), [this](void* data) { onCollisionEnter(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionStayEventKey(), [this](void* data) { onCollisionStay(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionExitEventKey(), [this](void* data) { onCollisionExit(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent("addScore", [this](void* data) { addScore(*static_cast<int*>(data)); });
}

void Player::update() noexcept
{
	if (_destroyFlag)
		return;

	//update呼び出しが必要なコンポーネントのupdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callUpdate();
}

void Player::fixedUpdate() noexcept
{
	if (_destroyFlag)
		return;

	//fixedUpdate呼び出しが必要なコンポーネントのfixedUpdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callFixedUpdate();

	if (isInvincible())
		_remainInvincibleTime -= CaramelEngine::Time::getFixedDeltaTime();

	TCHAR scoreStr[64];
	_stprintf_s(scoreStr, _T("SCORE : %d"), _currentScore);

	DrawString(20, 20, scoreStr, GetColor(255, 255, 255));
}

void Player::destroy() noexcept
{
	_destroyFlag = true;

	//イベントの解除
	Engine::getInstance().getEventDispathcer().removeEvent("onPressedKey");
	Engine::getInstance().getEventDispathcer().removeEvent("onHeldKey");
	Engine::getInstance().getEventDispathcer().removeEvent("onReleasedKey");
	Engine::getInstance().getEventDispathcer().removeEvent("onStickActiveL");
	Engine::getInstance().getEventDispathcer().removeEvent("onStickReleaseL");
	Engine::getInstance().getEventDispathcer().removeEvent("onStickActiveR");
	Engine::getInstance().getEventDispathcer().removeEvent("onStickReleaseR");
	Engine::getInstance().getEventDispathcer().removeEvent(getOnCollisionEnterEventKey());
	Engine::getInstance().getEventDispathcer().removeEvent(getOnCollisionStayEventKey());
	Engine::getInstance().getEventDispathcer().removeEvent(getOnCollisionExitEventKey());
}

void Player::onCollisionEnter(void* data) noexcept
{
	//衝突イベント情報にキャスト
	auto* event = static_cast<CollisionInfo*>(data);

	//衝突したものがどのグループに属するかで分岐
	switch ((eCollisionGroup)event->groupOther)
	{
	case eCollisionGroup::Item:

		getScrap(1);

		break;
	case eCollisionGroup::Enemy:

		if (isInvincible())
			return;

		_remainInvincibleTime = INVINCIBLETIME;

		auto renderer = findComponent<DXLibModelRender>();
		renderer->startBlink(60);

		break;
	}
}

void Player::onCollisionStay(void* data) noexcept
{

}

void Player::onCollisionExit(void* data) noexcept
{

}

void Player::onPressedKey(void* data) noexcept
{
	//キー入力情報にキャスト
	auto* keyData = static_cast<Cvector<InputKey>*>(data);
	Cvector<InputKey> keys = *keyData;
}

void Player::onHeldKey(void* data) noexcept
{
	//キー入力情報にキャスト
	auto* keyData = static_cast<Cvector<InputKey>*>(data);
	Cvector<InputKey> keys = *keyData;

	//移動用コンポーネントを捜索
	auto mover = findComponent<MovementComponent>();

	if (!mover)
		return;

	//移動方向の決定
	CVector3 moveDir = CVector3();

	for (auto key : keys)
	{
		//上キーか下キーが押されたら上下移動
		if (key == InputKey::W)
			moveDir += { 0.0f, 0.0f, 1.0f };
		else if (key == InputKey::S)
			moveDir += { 0.0f, 0.0f, -1.0f };

		if (key == InputKey::D)
			moveDir += { 1.0f, 0.0f, 0.0f };
		else if (key == InputKey::A)
			moveDir += { -1.0f, 0.0f, 0.0f };

		if (key == InputKey::Space || key == InputKey::ButtonA)
			throwScrap();
	}

	//移動方向なので正規化
	moveDir.normalized();

	//もし長さが0より大きければ
	if (moveDir.sqrtmagnitude() > 0)
	{
		_currentDir = moveDir;
		float targetYaw = atan2(moveDir.getX(), moveDir.getZ());
		auto currentEuler = _transform->getRotation().toEulerXYZ();
		CQuaternion targetRot = CQuaternion::fromEulerXYZ(currentEuler.getX(), targetYaw, currentEuler.getZ());
		CQuaternion newRot =
			CQuaternion::slerp(_transform->getRotation(), targetRot, _rotSpeed * CaramelEngine::Time::getFixedDeltaTime());

		_transform->setRotation(newRot);
	}

	//移動方向を移動用コンポーネントに伝える
	mover->setMoveDir(moveDir);
}

void Player::onReleasedKey(void* data) noexcept
{
	auto mover = findComponent<MovementComponent>();

	if (!mover)
		return;

	//キーが離されたら速度を0にする
	mover->setMoveDir(CVector3());
}

void Player::onStickActiveL(void* stick) noexcept
{
	auto* input = static_cast<StickInput*>(stick);

	auto mover = findComponent<MovementComponent>();

	if (!mover)
		return;

	auto moveDir = CVector3();

	if (std::abs(input->x) > 0.03)
		moveDir.setX(input->x);
	if (std::abs(input->y) > 0.03)
		moveDir.setZ(input->y);
	
	//もし長さが0より大きければ
	if (moveDir.sqrtmagnitude() > 0)
	{
		_currentDir = moveDir;
		float targetYaw = atan2(moveDir.getX(), moveDir.getZ());
		auto currentEuler = _transform->getRotation().toEulerXYZ();
		CQuaternion targetRot = CQuaternion::fromEulerXYZ(currentEuler.getX(), targetYaw, currentEuler.getZ());
		CQuaternion newRot =
			CQuaternion::slerp(_transform->getRotation(), targetRot, _rotSpeed * CaramelEngine::Time::getFixedDeltaTime());

		_transform->setRotation(newRot);
	}

	mover->setMoveDir(moveDir.normalized());
}

void Player::onStickReleaseL(void* stick) noexcept
{
	auto mover = findComponent<MovementComponent>();

	if (!mover)
		return;

	mover->setMoveDir(CVector3());
}

void Player::onStickActiveR(void* stick) noexcept
{
	auto* input = static_cast<StickInput*>(stick);

	CQuaternion delta =
		CQuaternion::fromAxisAngle(_transform->getUp(), input->x * _rotSpeed * CaramelEngine::Time::getFixedDeltaTime());
	_transform->setRotation(delta * _transform->getRotation());
}

void Player::onStickReleaseR(void* stick) noexcept
{
	auto mover = findComponent<MovementComponent>();

	if (!mover)
		return;

	mover->setMoveDir(CVector3());
}

void Player::getScrap(int addValue) noexcept
{
	if (_caughtScrap == nullptr)
		generateScrap();

	_caughtScrap->addScrap(addValue);

	
}

void Player::generateScrap()
{
	auto scrapTransform = std::make_shared<Transform>();
	scrapTransform->setPosition({ 0.0f, 1.f, 0.0f });
	scrapTransform->setScale({ 0.01f, 0.01f, 0.01f });
	scrapTransform->setParent(_transform.get());
	_caughtScrap = std::make_shared<CaughtScrap>(scrapTransform, "caughtScrap");
	_caughtScrap->initialize();

	Engine::getInstance().getSceneManager().getCurrentScene()->addActor(_caughtScrap);
}

void Player::throwScrap() noexcept
{
	if (_caughtScrap == nullptr)
		return;

	auto pos = _caughtScrap->getTransform()->getWorldPosition();
	_caughtScrap->throwScrap(_transform->getForward());
	_caughtScrap->getTransform()->setPosition(pos);
	_caughtScrap->getTransform()->setScale(_caughtScrap->getTransform()->getScale() * _transform->getScale());

	_caughtScrap.reset();
}