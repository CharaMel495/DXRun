#include "DxLib.h"
#include "CaramelEngine.h"
#include "MainScene.h"
#include "Player.h"
#include "DxLibLight.h"

using namespace DxLib;

using namespace CaramelEngine;

#define WINDOW_X 720
#define WINDOW_Y 480

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウ状態で起動(FSならFALSE)
    ChangeWindowMode(FALSE);
    // 解像度と色に使うビット数を指定
    SetGraphMode(WINDOW_X, WINDOW_Y, 32);
    // ライブラリの初期化
    if (DxLib_Init() == -1)
        return -1;
    // バックバッファに書き込むように指定
    SetDrawScreen(DX_SCREEN_BACK);

    auto engine = &Engine::getInstance();
    engine->initialize();

    engine->getSceneManager().addScene("MainScene", std::make_shared<MainScene>());
    engine->getSceneManager().switchScene("MainScene");

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