#pragma once
#include <map>
#include <string>
#include <variant>

#include "json/single_include/nlohmann/json.hpp"
#include "System/Math/Vector2.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

class Json{
    using json = nlohmann::json;
	using Value = std::variant<int32_t, float, Vector2, Vector3, Vector4>;
    using Object = std::map<std::string, Value>; //  key , value || key : [{ key, value }] // Item
    using Group = std::map<std::string, Object>; // group(ファイル内の一番浅い階層)


    const std::string PATH = "Assets/Data/";
    std::map<std::string, std::vector<Group>> datas_; // FileName, Data Groups

    //{
    //  "Data Key": [
    //      {
	//          "Group" : {
	//              "Object Key" : "Value"
	//          }
    //      },
    //      {
    //          "Group" : {
    //              "Object Key" : "Value"
    //          }
    //      }
    //  }
    //}

    // resume
    // {
    //     "FILE NAME" : {
    //          "KEY" : VALUE
    //     }
    // }

public:


    void SetValue(const std::string& _path, const std::string& _group, const std::string& _key, const Value& _value);
    Value GetValue(const std::string& _path, const std::string& group, const std::string& key) const;

    void Load(const std::string& _group);
	void Save(const std::string& _path);


private:
	void Register(const std::string& _name);
	void LoadJson(const std::string& _path);
};

