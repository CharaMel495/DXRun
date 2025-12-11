#include "Player.h"
#include "DXLibModelRender.h"
#include <cmath>

void Player::initialize() noexcept
{
	auto modelRender = addComponent<DXLibModelRender>();
	modelRender->setFilePath("Assets//Models//Test//GradiusExport.mv1");
	modelRender->initialize();
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

	//update呼び出しが必要なコンポーネントのupdateを呼び出し
	for (auto& comp : _myComponents)
		comp->callFixedUpdate();
}

void Player::destroy() noexcept
{
	_destroyFlag = true;
}