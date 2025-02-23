#pragma once

#include "map"
#include "functional"
#include "mutex"

class ImGuiCommand {
	std::mutex mutex_;
	std::map<void*,std::function<void()>> commands_;
public:
	void Update();

	void AddCommand(void* ptr, const std::function<void()>& command);
};