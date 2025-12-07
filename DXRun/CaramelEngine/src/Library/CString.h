#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// このエンジン用の文字列クラス
	/// </summary>
	class CString : public std::string
	{
	public:

		Cvector<CString> split(char delimiter);

		CString() : std::string() {}
		CString(const char* str) : std::string(str) {}
		CString(const std::string str) : std::string(str) {}
		CString& operator=(const char* str)
		{
			std::string::operator=(str);
			return *this;
		}
		CString& operator=(const std::string& str)
		{
			std::string::operator=(str);
			return *this;
		}

	private:

		
	};
}

// std::hashの特殊化
namespace std
{
	template<>
	struct hash<CaramelEngine::CString>
	{
		std::size_t operator()(const CaramelEngine::CString& str) const
		{
			return hash<std::string>()(str);  // 基本的にはstd::stringのハッシュを使う
		}
	};
}