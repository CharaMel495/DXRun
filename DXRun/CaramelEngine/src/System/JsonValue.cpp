#include "pch.h"

namespace CaramelEngine
{
    // 各コンストラクタは型を設定し、それに対応する値を代入
    JsonValue::JsonValue() : type(Type::Null) {}
    JsonValue::JsonValue(bool b) : type(Type::Boolean), boolValue(b) {}
    JsonValue::JsonValue(float n) : type(Type::Number), numberValue(n) {}
    JsonValue::JsonValue(const CString& s) : type(Type::String), stringValue(s) {}
    JsonValue::JsonValue(CString&& s) : type(Type::String), stringValue(std::move(s)) {}
    JsonValue::JsonValue(const std::vector<JsonValue>& arr) : type(Type::Array), arrayValue(arr) {}
    JsonValue::JsonValue(const std::unordered_map<CString, JsonValue>& obj) : type(Type::Object), objectValue(obj) {}

    JsonValue::Type JsonValue::getType() const { return type; }

    bool JsonValue::asBool() const 
    {
        if (type != Type::Boolean) 
            throw std::runtime_error("Not a boolean");
        
        return boolValue;
    }

    float JsonValue::asNumber() const 
    {
        if (type != Type::Number) 
            throw std::runtime_error("Not a number");
        
        return numberValue;
    }

    int JsonValue::asInt() const 
    {
        return static_cast<int>(asNumber());
    }

    const CString& JsonValue::asString() const
    {
        if (type != Type::String) 
            throw std::runtime_error("Not a string");
        
        return stringValue;
    }

    const JsonValue& JsonValue::operator[](const CString& key) const
    {
        if (type != Type::Object)
            throw std::runtime_error("Not an object");

        auto it = objectValue.find(key);
        if (it == objectValue.end())
            throw std::runtime_error("Key not found: " + key);

        return it->second;
    }

    const JsonValue& JsonValue::operator[](size_t index) const 
    {
        if (type != Type::Array)
            throw std::runtime_error("Not an array");

        if (index >= arrayValue.size())
            throw std::runtime_error("Index out of range");

        return arrayValue[index];
    }

    bool JsonValue::hasKey(const CString& key) const
    {
        if (type != Type::Object)
            return false;

        return objectValue.find(key) != objectValue.end();
    }

    size_t JsonValue::size() const 
    {
        if (type == Type::Array)
            return arrayValue.size();
        if (type == Type::Object)
            return objectValue.size();
        return 0;
    }
}
