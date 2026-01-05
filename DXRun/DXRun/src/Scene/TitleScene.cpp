#include "TitleScene.h"
#include "DXLib.h"

void TitleScene::initialize() noexcept
{
	Engine::getInstance().getEventDispathcer().subscribeEvent("onPressedKey", [this](void* data) { onPressedKey(data); });
}

void TitleScene::update() noexcept
{
	
}

void TitleScene::fixedUpdate() noexcept
{
	auto text = "GARACTARUN";

	auto screenSize = Engine::getInstance().getScreenSize();
	float centerX = screenSize.getX() * 0.5f;
	float centerY = screenSize.getY() * 0.5f;

	int width = DxLib::GetDrawStringWidth(text, (int)strlen(text));
	int height = DxLib::GetFontSize(); // フォントの高さ

	float posX = centerX - width / 2;
	float posY = centerY - height / 2;

	posY -= height;

	DxLib::DrawString((int)posX, (int)posY, text, DxLib::GetColor(255, 255, 255));

	text = "Press A Button";

	width = DxLib::GetDrawStringWidth(text, (int)strlen(text));
	
	posX = centerX - width / 2;
	posY += height * 2.0f;

	DxLib::DrawString((int)posX, (int)posY, text, DxLib::GetColor(255, 255, 255));
}

void TitleScene::shutdown() noexcept
{
	Engine::getInstance().getEventDispathcer().removeEvent("onPressedKey");
}

void TitleScene::onPressedKey(void* data) noexcept
{
	//キー入力情報にキャスト
	auto* keyData = static_cast<Cvector<InputKey>*>(data);
	Cvector<InputKey> keys = *keyData;

	for (auto key : keys)
	{
		if (key == InputKey::ButtonA)
			Engine::getInstance().getSceneManager().switchScene("MainScene");
	}
}