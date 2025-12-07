#pragma once

namespace CaramelEngine 
{
    /// <summary>
    /// シーン管理クラス
    /// </summary>
    class SceneManager final
    {
    public:

        SceneManager() = default;
        ~SceneManager() = default;

        //初期化
        void initialize();

        //更新処理
        void update();

        //固定字更新処理
        void fixedUpdate();

        //終了処理
        void shutdown();

        //シーンの追加
        void addScene(const CString& name, std::shared_ptr<Scene> scene);

        //シーンの削除
        void removeScene(const CString& name);

        //シーンの切り替え
        void switchScene(const CString& name);

        //現在のシーンを取得（デバッグ用）
        std::shared_ptr<Scene> getCurrentScene() const { return _currentScene; }

    private:

        std::unordered_map<CString, std::shared_ptr<Scene>> _scenes;
        std::shared_ptr<Scene> _currentScene;

        //void logSceneSwitch(const CString& name);
    };

} // namespace CaramelEngine
