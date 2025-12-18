#pragma once

#include "Cvector.h"
#include "cstring.h"

namespace CaramelEngine
{
	/// <summary>
	/// イベント管理クラス
	/// </summary>
	class EventDispatcher
	{
	public:

		inline EventDispatcher() noexcept {}

		/// <summary>
		/// イベントを登録するメソッド
		/// </summary>
		/// <param name="eventName">イベント名</param>
		/// <param name="callBack">対応イベント</param>
		void subscribeEvent(const CString eventName, std::function<void(void*)> callBack);
		
		/// <summary>
		/// 指定のイベントを削除するメソッド
		/// </summary>
		/// <param name="eventName">削除したいイベント名</param>
		void removeEvent(const CString& eventName);

		/// <summary>
		/// 指定のイベントを発火させるメソッド
		/// </summary>
		/// <param name="eventName">発火させたいイベント</param>
		/// <param name="data">イベントに用いるデータ</param>
		void dispatchEvent(const CString& eventName, void* data);
	private:

		/// <summary>
		/// イベントリスナー配列
		/// </summary>
		std::unordered_map<CString, std::vector<std::function<void(void*)>>> _listeners;
	};
}