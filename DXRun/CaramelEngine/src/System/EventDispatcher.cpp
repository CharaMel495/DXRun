#include "pch.h"

namespace CaramelEngine
{
	void EventDispatcher::subscribeEvent(const CString eventName, std::function<void(void*)> callBack)
	{
		_listeners[eventName].emplace_back(callBack);
	}

	void EventDispatcher::removeEvent(const CString& eventName)
	{
		_listeners.erase(eventName);
	}


	void EventDispatcher::dispatchEvent(const CString& eventName, void* data)
	{
		//イベントの存在確認にはcountを用いる
		if (_listeners.count(eventName))
			for (auto& listener : _listeners[eventName])
				listener(data);//渡されたデータを使ってイベントを呼び出す
	}
}