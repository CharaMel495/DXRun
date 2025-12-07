#pragma once

namespace CaramelEngine
{
    /// <summary>
    /// JSON値の汎用コンテナ。プリミティブ型（null, bool, number, string）および
    /// コンテナ型（object, array）を表現する。再帰構造を持ち、ツリー構造として使える。
    /// </summary>
    class JsonValue
    {
    public:
        /// <summary>
        /// JsonValueが保持する値の種類。
        /// </summary>
        enum class Type
        {
            Null,
            Boolean,
            Number,
            String,
            Object,
            Array
        };

        JsonValue(); // Nullとして初期化
        JsonValue(bool b);
        JsonValue(float n);
        JsonValue(const CString& s);
        JsonValue(CString&& s);
        JsonValue(const std::vector<JsonValue>& arr);
        JsonValue(const std::unordered_map<CString, JsonValue>& obj);

        /// <summary>
        /// 値の型を取得
        /// </summary>
        Type getType() const;

        /// <summary>
        /// boolean値として取得（型が違うと例外）
        /// </summary>
        bool asBool() const;
        /// <summary>
        /// 数値として取得（型が違うと例外）
        /// </summary>
        float asNumber() const;
        /// <summary>
        /// intとして取得（asNumberをintにキャスト）
        /// </summary>
        int asInt() const;
        /// <summary>
        /// 文字列として取得（型が違うと例外）
        /// </summary>
        const CString& asString() const;

        /// <summary>
        /// オブジェクトとしてキー指定アクセス（型違い/キーなしは例外）
        /// </summary>
        const JsonValue& operator[](const CString& key) const;
        /// <summary>
        /// 配列としてインデックスアクセス（型違い/範囲外は例外）
        /// </summary>
        const JsonValue& operator[](size_t index) const;

        /// <summary>
        /// オブジェクトに指定キーがあるか確認
        /// </summary>
        bool hasKey(const CString& key) const;
        /// <summary>
        /// 配列の要素数またはオブジェクトのキー数を返す
        /// </summary>
        size_t size() const;

    private:
        Type type = Type::Null;
        bool boolValue = false;
        float numberValue = 0.0;
        CString stringValue;
        std::unordered_map<CString, JsonValue> objectValue;
        std::vector<JsonValue> arrayValue;
    };
}
