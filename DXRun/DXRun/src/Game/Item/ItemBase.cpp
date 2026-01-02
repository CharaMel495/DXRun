#include "ItemBase.h"
#include "DXLibModelRender.h"

// --- MonoBehaviour ---
void ItemBase::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("temp//Models//Test//sphere.mv1");
	modelRender->initialize();

	auto sphere = addComponent<CSphere>();
	sphere->setTransform(_transform);
	_col = sphere;

	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionEnterEventKey(), [this](void* data) { onCollisionEnter(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionStayEventKey(), [this](void* data) { onCollisionStay(data); });
	Engine::getInstance().getEventDispathcer().subscribeEvent(getOnCollisionExitEventKey(), [this](void* data) { onCollisionExit(data); });
}

void ItemBase::update() noexcept
{
	if (_destroyFlag)
		return;

	//update呼び出しが必要なコンポーネントのupdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callUpdate();
}

void ItemBase::fixedUpdate() noexcept
{
	if (_destroyFlag)
		return;

	//fixedUpdate呼び出しが必要なコンポーネントのfixedUpdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callFixedUpdate();
}

void ItemBase::destroy() noexcept
{
	_destroyFlag = true;
}

void ItemBase::onCollisionEnter(void* data) noexcept
{
	//衝突イベント情報にキャスト
	auto* event = static_cast<CollisionInfo*>(data);

	//衝突したものがどのグループに属するかで分岐
	switch ((eCollisionGroup)event->groupOther)
	{
	case eCollisionGroup::Player:

		destroy();

		break;
	}
}

void ItemBase::onCollisionStay(void* data) noexcept
{

}

void ItemBase::onCollisionExit(void* data) noexcept
{

}