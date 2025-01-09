#include "ThreadManager.h"

#include <cassert>

#include "System/SingletonFinalizer/SingletonFinalizer.h"

ThreadManager* ThreadManager::instance = nullptr;
std::once_flag ThreadManager::flag;

ThreadManager::ThreadManager(size_t workers):exit(false) {
	for (size_t i = 0; i < workers; ++i){
		worker_.emplace_back(&ThreadManager::Work, this);
	}
}

ThreadManager::~ThreadManager() {
	Exit();
}

void ThreadManager::AddTask(std::function<void()> task) {
	std::lock_guard<std::mutex> lock(mutex_);
	tasks_.push(std::move(task));
	condition_.notify_all();
}

void ThreadManager::Exit() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		exit = true;
	}

	condition_.notify_all();

	for (auto& thread : worker_){
		if (!thread.joinable()) continue;
		thread.join();
	}
}

void ThreadManager::Work() {
	while (true){
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(mutex_);
			condition_.wait(lock, [this](){ return !tasks_.empty() || exit; });
			if (exit && tasks_.empty()) return;

			task = std::move(tasks_.front());
			tasks_.pop();
		}
		task();
	}
}

void ThreadManager::Create() {
	instance = new ThreadManager(4);
	SingletonFinalizer::AddFinalizer(&Destroy);
}

void ThreadManager::Destroy() {
	delete instance;
	instance = nullptr;
}

ThreadManager* ThreadManager::GetInstance() {
	std::call_once(flag, Create);
	assert(instance);
	return instance;
}
