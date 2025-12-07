#include "pch.h"

using namespace CaramelEngine;

Cvector<CString> CString::split(char delimiter)
{
	//返却用の配列を宣言
	std::vector<CString> vec;

	//分割したもののストリーム出力を受け取るやつを宣言
	std::stringstream ss{ *this };
	//切り出された文字列を格納する変数を宣言
	CString buf;
	//切り取ってvectorに格納していく
	while (std::getline(ss, buf, delimiter)) 
		vec.emplace_back(buf);

	return vec;
}