#include "pch.h"

namespace CaramelEngine 
{
    // 内部クラスの実体
    class Engine::Impl
    {
    public:

        Impl(Engine* owner) : _owner(owner)
        {
            if (_engine)
            {
                throw std::runtime_error("Engine is a singleton and already exists!");
            }

            _engine = this;
            logInfo("Engine::Impl created", __FILE__, __LINE__, __func__);
        }

        ~Impl()
        {
            _engine = nullptr;
            logInfo("Engine::Impl destroyed", __FILE__, __LINE__, __func__);
        }

        Impl(Impl&&) = default;
        Impl& operator=(Impl&&) = default;

        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;

        static Impl* getEngineInstance()
        {
            if (!_engine)
            {
                throw std::runtime_error("Engine instance is not initialized!");
            }
            return _engine;
        }

        Engine* _owner;

    private:

        static Impl* _engine;
    };

    // シングルトンインスタンスの取得
    Engine& Engine::getInstance()
    {
        static Engine instance;
        return instance;
    }

    void Engine::initialize()
    {
        if (_isInitialized) 
            return;

        _eventDispatcher = std::make_unique<EventDispatcher>();

        _scene = std::make_unique<SceneManager>();
        _stage = std::make_unique<StageManager>(_eventDispatcher.get());
        //_audio = std::make_unique<AudioManager>();

        _scene->initialize();
        _stage->initialize();
        //_audio->initialize();

        _isInitialized = true;
    }

    void Engine::update() noexcept
    {
        if (!_isInitialized) 
            return;

        float deltaTime = 1.0f / 60.0f; // 現状仮。将来的に時間差で計算してもOK
        _fixedTimer += deltaTime;

        while (_fixedTimer >= _fixedDeltaTime)
        {
            fixedUpdate();
            _fixedTimer -= _fixedDeltaTime;
        }

        //_audio->update();
        _scene->update();
        _colliderManager->update();
    }

    void Engine::fixedUpdate() noexcept
    {
        if (!_isInitialized)
            return;

        // 固定ステップで行いたい更新処理（物理・点滅など）
        _scene->fixedUpdate();  // ← SceneManagerに実装必要

        // ステージの進行を更新
        _stage->fixedUpdate();
    }

    void Engine::shutdown() noexcept
    {
        if (!_isInitialized) 
            return;

        //_audio->shutdown();
        _scene->shutdown();

        _scene.reset();
        _eventDispatcher.reset();

        _isInitialized = false;
    }

    void Engine::registerToCollisionManager(uint32_t groupId, std::shared_ptr<ICollisionObject> obj) noexcept
    {
        if (_colliderManager)
            _colliderManager->registerObject(groupId, obj);
    }

    void Engine::unregisterFromCollisionManager(std::shared_ptr<ICollisionObject> obj) noexcept
    {
        if (_colliderManager)
            _colliderManager->unregisterObject(obj);
    }
}