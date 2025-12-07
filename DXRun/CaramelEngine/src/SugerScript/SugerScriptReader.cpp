#include "pch.h"
#include "SugerScriptReader.h"
#include "SugerScriptFunctions.h"
#include "ExchangeEnumWithString.h"
#include <fstream>

using namespace SugerScript;
using namespace CaramelEngine;

SugerScriptReader::SugerScriptReader(CString scriptPath)
	:_path(scriptPath)
{

}

void SugerScriptReader::Initialize()
{

}

void SugerScriptReader::AnalysisSuger()
{
	std::ifstream file;

	file.open(_path, std::ios::in);

	CString line;

	while (std::getline(file, line)) 
	{
		AnalysisSugerLine(line);
	}

	file.close();
}

void SugerScriptReader::AnalysisSugerLine(CString text)
{
	//空白行だった場合何も読まない
	if (text.size() < 1)
		return;

	//コメントアウトを検知したら何も読まない
	if (text.at(0) == '/')
		return;

	//'|'をデリミタとして分割
	std::vector<CString> strArray = text.split('|');

	//要求を取得
	CString request = strArray[(int)SplitOrder::Request];
	//型名を取得
	CString typeName = strArray[(int)SplitOrder::TypeName];
	//変数名を取得
	CString fieldName = strArray[(int)SplitOrder::FieldName];
	//もし初期化値があればそれを入れる
	if (strArray.size() >= (int)SplitOrder::InitialParameter)
		CString fieldName = strArray[(int)SplitOrder::InitialParameter];


	CreateField(_dataTypes[fieldName]);
}

ISuger& SugerScriptReader::CreateField(DataType dataType) const
{
	switch (dataType)
	{
	case DataType::Int:
	{
		auto field = Functions::CreateIntegerField();
		return *field;
	}
		break;

	case DataType::Float:
	{
		auto field = Functions::CreateFloatField();
		return *field;
	}
		break;
	}

	//TODO エラーログを書く

	ISuger hoge;

	return hoge;
}