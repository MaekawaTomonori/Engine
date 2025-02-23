#pragma once

#include "thread"
#include "vector"
#include "queue"
#include "mutex"
#include "functional"
#include "condition_variable"
#include "atomic"

class ThreadManager {

public:

	ThreadManager(size_t threadCount);

	~ThreadManager();

	void AddTask(std::function<void()> task);

	void Stop();

	bool StandByAllThread();

	int GetWorkingThread() { return workingThread_; }

private:

	void WorkerThread();

private:

	//スレッドリスト
	std::vector<std::thread> threads_;

	//タスクキュー
	std::queue<std::function<void()>> taskQueue;

	//ロック
	std::mutex queueMutex;

	//スレッドの待機、実行状態を管理
	std::condition_variable condition;

	//終了フラグ
	std::atomic<bool> stopFlag;

	std::atomic<int> workingThread_;
};