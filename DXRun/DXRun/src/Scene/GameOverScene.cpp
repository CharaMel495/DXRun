#include "GameOverScene.h"

void GameOverScene::initialize() noexcept
{

}

void GameOverScene::update() noexcept
{
	auto text = "Game Over";

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

void GameOverScene::fixedUpdate() noexcept
{

}

void GameOverScene::shutdown() noexcept
{

}


void GameOverScene::onPressedKey(void* data) noexcept
{
	//キー入力情報にキャスト
	auto* keyData = static_cast<Cvector<InputKey>*>(data);
	Cvector<InputKey> keys = *keyData;

	for (auto key : keys)
	{
		if (key == InputKey::ButtonA)
			Engine::getInstance().getSceneManager().switchScene("TitleScene");
	}
}