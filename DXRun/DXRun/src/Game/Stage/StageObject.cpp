#include "StageObject.h"
#include "DXLibModelRender.h"
#include "FunctionDefinition.h"
#include "ItemBase.h"

// --- MonoBehaviour ---
void StageObject::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("temp//Models//Test//testStageA.mv1");
	modelRender->initialize();
	_itemNum = 0;
	_itemTimer = 0.0f;
}

void StageObject::update() noexcept
{
	if (_destroyFlag)
		return;

	//update呼び出しが必要なコンポーネントのupdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callUpdate();
}

void StageObject::fixedUpdate() noexcept
{
	if (_destroyFlag)
		return;

	//fixedUpdate呼び出しが必要なコンポーネントのfixedUpdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callFixedUpdate();

	_itemTimer += CaramelEngine::Time::getFixedDeltaTime();

	if (_itemTimer < _itemPopInterval)
		return;

	_itemTimer = 0.0f;
	popItem();
}

void StageObject::destroy() noexcept
{
	_destroyFlag = true;
}

void StageObject::popItem()
{
	CVector3 pos = _transform->getPosition();
	CVector3 scale = _transform->getScale();
	CVector3 halfScale = scale * 0.5f;
	float posX = getRandomFloat(pos.getX() - halfScale.getX(), pos.getX() + halfScale.getX());
	float posY = pos.getY () + halfScale.getY();
	float posZ = getRandomFloat(pos.getZ() - halfScale.getZ(), pos.getZ() + halfScale.getZ());
	CVector3 itemScale = { 100.0f, 100.0f, 100.0f };
	posY = 240.0f;
	CVector3 itemPos = { posX, posY, posZ };
	auto itemTransform = std::make_shared<Transform>();
	itemTransform->setPosition(itemPos);
	itemTransform->setScale(itemScale);
	auto item = std::make_shared<ItemBase>(itemTransform, "item_" + std::to_string(_itemNum));
	item->initialize();

	++_itemNum;
	
	Engine::getInstance().getSceneManager().getCurrentScene()->addActor(item);
}

void StageObject::popEnemy()
{

}