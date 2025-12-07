#include "pch.h"

namespace CaramelEngine 
{
    void SceneManager::addScene(const CString& name, std::shared_ptr<Scene> scene) 
    {
        _scenes[name] = scene;
        logInfo("Scene added: " + name, __FILE__, __LINE__, __FUNCTION__);
    }

    void SceneManager::removeScene(const CString& name) 
    {
        auto it = _scenes.find(name);
        if (it != _scenes.end()) 
        {
            _scenes.erase(it);
            logInfo("Scene removed: " + name, __FILE__, __LINE__, __FUNCTION__);
        }
        else 
        {
            logWarning("Attempted to remove non-existent scene: " + name, __FILE__, __LINE__, __FUNCTION__);
        }
    }

    void SceneManager::switchScene(const CString& name) 
    {
        auto it = _scenes.find(name);
        if (it != _scenes.end()) 
        {
            if (_currentScene) 
            {
                _currentScene->shutdown();
            }
            _currentScene = it->second;
            _currentScene->initialize();
            logInfo("Switched to scene: " + name, __FILE__, __LINE__, __FUNCTION__);
        }
        else 
        {
            logError("Scene not found: " + name, __FILE__, __LINE__, __FUNCTION__);
        }
    }

    void SceneManager::update() 
    {
        if (_currentScene) 
        {
            _currentScene->update();
        }
        else 
        {
            logWarning("No active scene to update.", __FILE__, __LINE__, __FUNCTION__);
        }
    }

    void SceneManager::fixedUpdate()
    {
        if (_currentScene)
        {
            _currentScene->fixedUpdate();
        }
        else
        {
            logWarning("No active scene to update.", __FILE__, __LINE__, __FUNCTION__);
        }
    }

    void SceneManager::initialize() 
    {
        if (_currentScene) 
        {
            _currentScene->initialize();
        }
    }

    void SceneManager::shutdown() 
    {
        if (_currentScene) 
        {
            _currentScene->shutdown();
        }
        _scenes.clear();
        logInfo("SceneManager shutdown.", __FILE__, __LINE__, __FUNCTION__);
    }

} // namespace CaramelEngine
