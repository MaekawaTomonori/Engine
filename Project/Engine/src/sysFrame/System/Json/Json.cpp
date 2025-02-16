#include "Json.h"

#include <fstream>

#include "Framework/Adapter.h"
#include "System/System.h"

void Json::Register(const std::string& name) {
    if (datas_.contains(name))return;

    datas_[name];
}

void Json::LoadJson(const std::string& _path) {
    //Open File
    std::string path = PATH + _path + ".json";
    std::ifstream file(path);
    if (!file.good()){
        System::Log(Log::Level::ERR, "Failed open file for read");
        return;
    }

    json root;
    file >> root;
    file.close();

    // Load Data
    auto data = root.find(_path);
    assert(data != root.end());

    for (auto group = data->begin(); group != data->end(); ++group){
    	const std::string& groupKey = group.key();

        for (auto item = group->begin(); item != group->end(); ++item){
            const std::string& key = item.key();
            const json& value = item.value();
            if (value.is_number_integer()){
                int32_t v = value.get<int32_t>();
                SetValue(_path, groupKey, key, v);
            } else if (value.is_number_float()){
                float v = value.get<float>();
                SetValue(_path, groupKey, key, v);
            } else if (value.is_array()){
                if (value.size() == 2){
                    Vector2 v = {value[0].get<float>(), value[1].get<float>()};
                    SetValue(_path, groupKey, key, v);
                } else if (value.size() == 3){
                    Vector3 v = {value[0].get<float>(), value[1].get<float>(), value[2].get<float>()};
                    SetValue(_path, groupKey, key, v);
                } else if (value.size() == 4){
                    Vector4 v = {value[0].get<float>(), value[1].get<float>(), value[2].get<float>(), value[3].get<float>()};
                    SetValue(_path, groupKey, key, v);
                }
            }
        }
    }
}

void Json::SetValue(const std::string& _path, const std::string& _group, const std::string& _key, const Value& _value) {
    Register(_path);

    auto& data = datas_[_path];

    Object object;
    object[_key] = _value;

    Group group;
	group[_group] = object;

    data.push_back(group);
}

Json::Value Json::GetValue(const std::string& _path, const std::string& _group, const std::string& _key) const {
    (void)_path;
    (void)_group;
    (void)_key;
    return {};
}

void Json::Load(const std::string& _group) {
    std::filesystem::path dir(PATH + _group + "/");
    if (!exists(dir)){
        return;
    }

    std::filesystem::directory_iterator itr(dir);
    for (const auto& entry : itr){
        const std::filesystem::path& path = entry.path();

        std::string extension = path.extension().string();
    	if (extension != ".json"){
            continue;
        }

        LoadJson(path.stem().string());
    }
}

void Json::Save(const std::string& _path) {
    auto data = datas_.find(_path);
    assert(data != datas_.end());

    json root = json::object();
    root[_path] = json::array();
    json groupData = json::object();

    for (auto group = data->second.begin(); group != data->second.end(); ++group){
        std::string groupKey = group->begin()->first;
        for (auto object = group->begin(); object != group->end(); ++object){
            const Object& item = object->second;

            for (auto [key, value] : item){
                if (std::holds_alternative<int32_t>(value)){
                    groupData[groupKey][key] = std::get<int32_t>(value);
                } else if (std::holds_alternative<float>(value)){
                    groupData[groupKey][key] = std::get<float>(value);
                } else if (std::holds_alternative<Vector2>(value)){
                    Vector2 v = std::get<Vector2>(value);
                    groupData[groupKey][key] = {v.x, v.y};
                } else if (std::holds_alternative<Vector3>(value)){
                    Vector3 v = std::get<Vector3>(value);
                    groupData[groupKey][key] = {v.x, v.y, v.z};
                } else if (std::holds_alternative<Vector4>(value)){
                    Vector4 v = std::get<Vector4>(value);
                    groupData[groupKey][key] = {v.x, v.y, v.z, v.w};
                }
                root[_path].push_back(groupData);
            }
        }
    }

    std::filesystem::path dir(PATH + _path + "/");
    if (!exists(dir)){
        create_directories(dir);
    }

    std::string path = dir.string() + _path + ".json";
	std::ofstream file;
	file.open(path);

    if (file.fail()){
	    System::Log(Log::Level::ERR, "Failed open file for write");
        return;
    }

    file << std::setw(4) << root << '\n';
    file.close();
}
