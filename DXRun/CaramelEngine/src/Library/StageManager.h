#pragma once

namespace CaramelEngine
{
    class StageManager
    {
    public:
        inline StageManager() noexcept = delete;
        StageManager(EventDispatcher* dispatcher) :
            _dispatcher(dispatcher)
        {}

        void initialize();

        /// <summary>
        /// ステージスクリプト（JSON）を読み込む
        /// </summary>
        void loadStageScript(const CString& path);

        /// <summary>
        /// 毎フレーム呼び出す。イベントを発火する。
        /// </summary>
        void fixedUpdate();

        /// <summary>
        /// ステージ状態をリセット（リトライ用）
        /// </summary>
        void reset();

        /// <summary>
        /// ステージが終了したかどうか
        /// </summary>
        bool isStageEnd() const;

        void subscribeEnemySpawnEvent(std::function<void(void*)> func)
        {
            _dispatcher->subscribeEvent("spawnEnemy", func);
        }

        struct StageEvent
        {
            int frame = 5;
            CString type;
            std::unordered_map<std::string, std::string> payload;
        };
    private:

        EventDispatcher* _dispatcher;
        Cvector<StageEvent> _events;
        size_t _currentEventIndex = 0;
        int _currentFrame = 0;
        bool _stageEnded = false;
        CString _currentScriptPath;

        void dispatchStageEvent(const StageEvent& e);
    };
}
#pragma once
