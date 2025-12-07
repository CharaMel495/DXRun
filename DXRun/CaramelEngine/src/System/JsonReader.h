#pragma once

namespace CaramelEngine
{
    /// <summary>
    /// JSON文字列 or ファイルを構文解析して JsonValue ツリーを構築する簡易パーサー
    /// </summary>
    class JsonReader
    {
    public:
        /// <summary>
        /// ファイルからJSONを読み込み、ルートノードを構築
        /// </summary>
        bool parseFromFile(const CString& path);

        /// <summary>
        /// 文字列からJSONを読み込み、ルートノードを構築
        /// </summary>
        bool parseFromString(const CString& jsonString);

        /// <summary>
        /// 解析済みのルートオブジェクトを取得
        /// </summary>
        const JsonValue& root() const;

    private:
        JsonValue _rootValue;

        //以下、再帰下降構文解析用のヘルパー関数群
        const char* parseValue(const char* p, JsonValue& out);
        const char* parseObject(const char* p, JsonValue& out);
        const char* parseArray(const char* p, JsonValue& out);
        const char* parseString(const char* p, CString& out);
        const char* parseNumber(const char* p, float& out);
        const char* parseLiteral(const char* p, JsonValue& out);
        const char* skipWhitespace(const char* p);
    };
}
