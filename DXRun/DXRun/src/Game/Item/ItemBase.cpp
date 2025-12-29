#include "ItemBase.h"
#include "DXLibModelRender.h"

// --- MonoBehaviour ---
void ItemBase::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("Assets//Models//Test//sphere.mv1");
	modelRender->initialize();
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