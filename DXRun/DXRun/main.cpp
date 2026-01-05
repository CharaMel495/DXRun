#include "DxLib.h"
#include "CaramelEngine.h"
#include "MainScene.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "Player.h"
#include "DxLibLight.h"
#include "ResourceLoader.h"

using namespace DxLib;

using namespace CaramelEngine;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // リソースのロード
    HRSRC hRes = ExtractAssetsToTemp();

    if (!hRes)
        return 1;

    // まずはキャラメルエンジンのセットアップ
    auto engine = &Engine::getInstance();
    engine->initialize();
    auto screenSize = engine->getScreenSize();

    // ウィンドウ状態で起動(FSならFALSE)
    ChangeWindowMode(TRUE);
    // 解像度と色に使うビット数を指定
    SetGraphMode((int)screenSize.getX(), (int)screenSize.getY(), 32);
    // ライブラリの初期化
    if (DxLib_Init() == -1)
        return -1;
    // バックバッファに書き込むように指定
    SetDrawScreen(DX_SCREEN_BACK);

    engine->getSceneManager().addScene("TitleScene", std::make_shared<TitleScene>());
    engine->getSceneManager().addScene("MainScene", std::make_shared<MainScene>());
    engine->getSceneManager().addScene("GameOverScene", std::make_shared<GameOverScene>());
    engine->getSceneManager().switchScene("TitleScene");

    // 真上から照らすライトを生成
    LightDesc light;
    light.type = LightType::Directional;
    light.position = VGet(0.0f, 1500.0f, 0.0f);
    light.direction = VGet(0.0f, 1.0f, 0.2f);
    light.diffuse = { 1.0f, 0.95f, 0.9f, 1.0f };

    DxLibLight dxLight;
    dxLight.Apply(light);


    // エラーメッセージが飛んで来たらループ終了
    while (ProcessMessage() != -1) 
    {
        int startTime = GetNowCount();
        // バックバッファに書き込んでいたものを表に出す
        ScreenFlip();
        // バックバッファを綺麗にする
        ClearDrawScreen();

        engine->update();

        // Escキーでもループを抜けられるように
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
            break;

        int endTime = GetNowCount();
    }

    Engine::getInstance().shutdown();

    DxLib_End();
    return 0;
}