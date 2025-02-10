#include "ImGuiCommand.h"

#include <algorithm>
#include <functional>
#include <ranges>

void ImGuiCommand::Update() {

	std::vector<std::pair<void*, std::function<void()>>> vec(commands_.begin(), commands_.end());

	std::ranges::sort(vec, [](const auto& a, const auto& b) {
		return a.first < b.first;
	});

	for (auto& val : vec | std::views::values) {
		val();
	}

	commands_.clear();
}

void ImGuiCommand::AddCommand(void* ptr, const std::function<void()>& command) {

	{
		std::unique_lock<std::mutex> lock(mutex_);

		commands_.insert(std::make_pair(ptr, command));

	}
}