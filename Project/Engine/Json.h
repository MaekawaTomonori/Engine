#pragma once
#include <map>
#include <string>
#include <variant>

#include "System/Math/Vector2.h"
#include "System/Math/Vector3.h"
#include "System/Math/Vector4.h"

class Json{
	struct Item{
        std::variant<int32_t, float, Vector2, Vector3, Vector4> value;
	};

	struct Group{
		std::map<std::string, Item> items;
	};

    std::map<std::string, Group> datas_;
};

