#pragma once
#include "StageManager.h"
#include "SceneManager.h"

namespace CaramelEngine 
{
    namespace Time
    {
        inline float getFixedDeltaTime() { return 1.0f / 60.0f; }
    }

    /// <summary>
    /// ゲーム全体を管理するエンジンクラス
    /// </summary>
    class Engine 
    {
    public:

        static Engine& getInstance();

        void initialize();
        void update() noexcept;
        void fixedUpdate() noexcept;
        void shutdown() noexcept;

        SceneManager& getSceneManager() const { return *_scene; }
        //AudioManager& getAudio() { return *_audio; }
        EventDispatcher& getEventDispathcer() const { return *_eventDispatcher; }

        void setColliderManager(std::shared_ptr<ColliderManagerBase> mgr) noexcept 
        { 
            _colliderManager = mgr; 
            _colliderManager->initialize();
        }
        void registerToCollisionManager(uint32_t groupId, std::shared_ptr<ICollisionObject> obj) noexcept;
        void unregisterFromCollisionManager(std::shared_ptr<ICollisionObject> obj) noexcept;

        CVector2 getScreenSize() const noexcept { return _screenSize; }

    private:

        class Impl;

        Engine() = default;
        ~Engine() = default;

        float _fixedDeltaTime = 1.0f / 60.0f;
        float _fixedTimer = 0.0f;

        std::unique_ptr<EventDispatcher> _eventDispatcher;

        std::unique_ptr<SceneManager> _scene;
        //std::unique_ptr<AudioManager> _audio;
        std::shared_ptr<ColliderManagerBase> _colliderManager;

        bool _isInitialized = false;

        CVector2 _screenSize = { 1280.0f, 720.0f }; // 仮。起動時に初期化される想定
    };

} //