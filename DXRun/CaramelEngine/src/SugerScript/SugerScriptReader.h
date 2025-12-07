#pragma once

#include "CString.h"

namespace SugerScript
{
	/// <summary>
	/// Suger(自作言語)の文字解析を行うクラス
	/// </summary>
	class SugerScriptReader
	{
	public :

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="scriptPath">この解析機が取り扱うファイル</param>
		SugerScriptReader(CaramelEngine::CString scriptPath);

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// ファイルに書かれたSugerをC++の関数に変換してstd::functionにまとめて返す
		/// </summary>
		/// <returns>std::function<void>型に直されたSugerプログラム</returns>
		void AnalysisSuger();

		/// <summary>
		/// 読み込んだ１行を解釈する関数
		/// </summary>
		/// <param name="text">読み込んだ１行</param>
		/// <returns>行うべき関数</returns>
		void AnalysisSugerLine(CaramelEngine::CString text);

	private :

		SugerScriptReader() {}

		/// <summary>
		/// 読み込むスクリプトが置かれているディレクトリ
		/// </summary>
		CaramelEngine::CString _path;

		enum struct RequestType
		{
			Define,
			Calculate,
		};

		enum struct DataType
		{
			Int,
			Float,
			Double,
			Boolean
		};

		enum struct SplitOrder
		{
			Request,
			TypeName,
			FieldName,
			InitialParameter,
		};

		/// <summary>
		/// 要求されたデータ型
		/// </summary>
		std::map<std::string, DataType> _dataTypes
		{
			{"int", DataType::Int},
			{"float", DataType::Float},
			{"double", DataType::Double},
			{"bool", DataType::Boolean},
		};

		/// <summary>
		/// 要求されたリクエスト
		/// </summary>
		std::map<std::string, RequestType> _requestTypes
		{
			{":def", RequestType::Define},
			{":calc", RequestType::Calculate},
		};

		ISuger& CreateField(DataType dataType) const;
	};
}