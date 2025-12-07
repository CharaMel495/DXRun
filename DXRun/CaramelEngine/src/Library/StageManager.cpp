#include "pch.h"

namespace CaramelEngine
{
    void StageManager::initialize()
    {
        reset();
    }

    void StageManager::loadStageScript(const CString& path)
    {
        _events.clear();
        _currentEventIndex = 0;
        _currentFrame = 0;
        _stageEnded = false;
        _currentScriptPath = path;

        JsonReader reader;
        if (!reader.parseFromFile(path))
        {
            std::cerr << "[StageManager] Failed to load script: " << path << std::endl;
            return;
        }

        const auto& root = reader.root();
        for (size_t i = 0; i < root.size(); ++i)
        {
            const auto& item = root[i];
            StageEvent e;
            e.frame = item["frame"].asInt();
            e.type = item["type"].asString();

            for (const auto& key : { "motionId", "enemyId", "dialogueId", "x", "y", "dirX", "dirY" })
            {
                if (item.hasKey(key))
                {
                    e.payload[key] = item[key].asString(); // stringŒÀ’è‚Åˆµ‚¤
                }
            }

            _events.emplace_back(e);
        }
    }

    void StageManager::fixedUpdate()
    {
        if (_stageEnded) return;

        while (_currentEventIndex < _events.size() &&
            _events[_currentEventIndex].frame <= _currentFrame)
        {
            dispatchStageEvent(_events[_currentEventIndex]);
            _currentEventIndex++;
        }

        _currentFrame++;

        if (_currentEventIndex >= _events.size())
        {
            _stageEnded = true;
        }
    }

    void StageManager::dispatchStageEvent(const StageEvent& e)
    {
        _dispatcher->dispatchEvent(e.type, (void*)&e);
    }

    void StageManager::reset()
    {
        //loadStageScript(_currentScriptPath);
    }

    bool StageManager::isStageEnd() const
    {
        return _stageEnded;
    }
}
