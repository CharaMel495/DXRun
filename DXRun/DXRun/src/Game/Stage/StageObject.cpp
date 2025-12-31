#include "StageObject.h"
#include "DXLibModelRender.h"

// --- MonoBehaviour ---
void StageObject::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("Assets//Models//Test//testStageA.mv1");
	modelRender->initialize();
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
}

void StageObject::destroy() noexcept
{
	_destroyFlag = true;
}