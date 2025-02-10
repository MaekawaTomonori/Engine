#include "ThreadManager.h"

std::atomic<int> workingThread_(0);

ThreadManager::ThreadManager(size_t threadCount) : stopFlag(false) {

	for (size_t i = 0; i < threadCount; i++) {

		//スレッドを起動
		threads_.emplace_back(&ThreadManager::WorkerThread, this);
	}
}

ThreadManager::~ThreadManager() {

	//スレッドの停止
	Stop();
}

void ThreadManager::AddTask(std::function<void()> task) {
	{
		//スコープ内の処理が終わるまでmutexを占有
		std::lock_guard<std::mutex> lock(queueMutex);

		//キューにタスクを登録する
		taskQueue.push(std::move(task));
	}

	//待機中のスレッドをひとつだけ動かす
	condition.notify_one();
}

void ThreadManager::Stop() {
	{
		//スコープ内の処理が終わるまでmutexを占有
		std::lock_guard<std::mutex> lock(queueMutex);

		//終了フラグをtrueにする
		stopFlag = true;
	}

	//待機中のスレッドをすべて動かす
	condition.notify_all();

	for (std::thread& worker : threads_) {

		//スレッドの解放
		if (worker.joinable()) {
			worker.join();
		}
	}
}

bool ThreadManager::StandByAllThread() {

	if (workingThread_ > 0 || !taskQueue.empty()) {
		return false;
	}

	return true;
}

void ThreadManager::WorkerThread() {

	//無限ループ
	while (true) {

		//実行するタスク
		std::function<void()> task;

		{
			//スコープ内の処理が終わるまでmutexを占有
			std::unique_lock<std::mutex> lock(queueMutex);

			//ストップフラグがたつか、キューに何かが入ってくるまで処理を止める
			condition.wait(lock, [this]() {return stopFlag || !taskQueue.empty(); });

			//終了フラグがたち、キューが空であったら
			if (stopFlag && taskQueue.empty()) {

				//ループを抜ける
				return;
			}

			workingThread_++;

			//タスクキューから実行するタスクを取り出す
			task = std::move(taskQueue.front());

			//取り出したタスクをキューから削除する
			taskQueue.pop();
		}

		//タスクを実行
		task();

		workingThread_--;
	}
}
