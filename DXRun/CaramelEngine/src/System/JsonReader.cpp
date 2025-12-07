#include "pch.h"

namespace CaramelEngine
{
    //ファイルから全文読み込んで文字列として解析
    bool JsonReader::parseFromFile(const CString& path)
    {
        std::ifstream file(path);
        //ファイルを開けなければ終了
        if (!file) 
            return false;
        std::stringstream ss;
        ss << file.rdbuf();
        return parseFromString(ss.str());
    }

    //文字列からJSONを構文解析し、ルートに格納
    bool JsonReader::parseFromString(const CString& jsonString)
    {
        const char* p = jsonString.c_str();
        p = skipWhitespace(p);
        p = parseValue(p, _rootValue);
        return p != nullptr;
    }

    const JsonValue& JsonReader::root() const
    {
        return _rootValue;
    }

    const char* JsonReader::skipWhitespace(const char* p)
    {
        //ポインタを進めることで次の文字に進める
        while (*p && std::isspace(*p)) 
            ++p;
        return p;
    }

    const char* JsonReader::parseValue(const char* p, JsonValue& out)
    {
        //空白をスキップ
        p = skipWhitespace(p);
        //オブジェクト開始タグを見つけたら
        if (*p == '{') 
            return parseObject(p, out);
        //配列開始タグを見つけたら
        if (*p == '[') 
            return parseArray(p, out);
        
        //文字列解釈
        if (*p == '"') 
        {
            CString s;
            p = parseString(p, s);
            out = JsonValue(s);
            return p;
        }

        //実数解釈
        if (*p == '-' || std::isdigit(*p)) 
        {
            float n;
            p = parseNumber(p, n);
            out = JsonValue(n);
            return p;
        }

        return parseLiteral(p, out);
    }

    //{ ... } を解析し、オブジェクト構文を構築
    const char* JsonReader::parseObject(const char* p, JsonValue& out)
    {
        //'{'が来ているハズなのでスキップするためつぎへ移動
        ++p; 
        std::unordered_map<CString, JsonValue> obj;
        //空白スキップ
        p = skipWhitespace(p);
        //もし終了タグが来れば終了
        if (*p == '}')
            return ++p, out = JsonValue(obj), p;

        while (true)
        {
            //キー値を取得
            CString key;
            //空白スキップ
            p = skipWhitespace(p);
            //キーを文字列として取得
            p = parseString(p, key);
            //空白スキップ
            p = skipWhitespace(p);
            //もしjsonのフォーマットが崩れていればここで終了
            auto hoge = *p;
            if (*p != ':') 
                return nullptr;
            //バリュー値の取得
            ++p;
            JsonValue val;
            p = parseValue(p, val);
            //キーとバリューを紐づけ
            obj[key] = val;
            p = skipWhitespace(p);
            //終了タグが来れば終了
            if (*p == '}') 
                return ++p, out = JsonValue(obj), p;
            //もし','以外ならフォーマットが崩れてるので解釈終了
            auto fuga = *p;
            if (*p != ',') 
                return nullptr;
            //つぎへ移動
            ++p;
        }
    }

    //[ ... ] を解析し、配列構文を構築
    const char* JsonReader::parseArray(const char* p, JsonValue& out)
    {
        //'['は読み飛ばす
        ++p;
        Cvector<JsonValue> arr;
        //空白をスキップ
        p = skipWhitespace(p);
        //もし終了タグが来たらここで終了
        if (*p == ']') 
            return ++p, out = JsonValue(arr), p;

        while (true)
        {
            JsonValue val;
            //バリューをパース
            p = parseValue(p, val);
            //パースしたものを配列に格納
            arr.emplace_back(val);
            p = skipWhitespace(p);
            if (*p == ']') 
                return ++p, out = JsonValue(arr), p;
            if (*p != ',') 
                return nullptr;
            ++p;
        }
    }

    //"..." をパースし、必要に応じて \n, \t, \" 等を解釈
    const char* JsonReader::parseString(const char* p, CString& out)
    {
        //ダブルクォーテーションをスキップ
        ++p;
        //一度クリアしておく
        out.clear();
        while (*p && *p != '"')
        {
            //特殊文字の解釈
            if (*p == '\\')
            {
                ++p;
                if (*p == 'n') out += '\n';
                else if (*p == 't') out += '\t';
                else if (*p == 'r') out += '\r';
                else out += *p;
            }
            else
            {
                //通常の文字であればそのまま取り込む
                out += *p;
            }
            ++p;
        }
        //もしダブルクォーテーションで終了してなければフォーマット違反なのでnullptrを返す
        return *p == '"' ? ++p : nullptr;
    }

    //数字（整数・小数）を double に変換
    const char* JsonReader::parseNumber(const char* p, float& out)
    {
        char* end = nullptr;
        out = std::strtof(p, &end);
        return (end != p) ? end : nullptr;
    }

    //true, false, null の3種リテラルを識別
    const char* JsonReader::parseLiteral(const char* p, JsonValue& out)
    {
        //正規表現で判定していく
        if (std::strncmp(p, "true", 4) == 0 || std::strncmp(p, "True", 4) == 0 || std::strncmp(p, "TRUE", 4) == 0)
        {
            out = JsonValue(true);
            return p + 4;
        }
        if (std::strncmp(p, "false", 5) == 0 || std::strncmp(p, "False", 5) == 0 || std::strncmp(p, "FALSE", 5) == 0)
        {
            out = JsonValue(false);
            return p + 5;
        }
        if (std::strncmp(p, "null", 4) == 0 || std::strncmp(p, "Null", 4) == 0 || std::strncmp(p, "NULL", 4) == 0)
        {
            out = JsonValue();
            return p + 4;
        }
        return nullptr;
    }
}
