#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadManager{
    std::vector<std::thread> worker_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<bool> exit;
private:
    void  Work();

public:
	void AddTask(std::function<void()> task);

    void Exit();


private:
    ThreadManager(size_t workers);
    ~ThreadManager();

    static ThreadManager* instance;
    static std::once_flag flag;

    static void Create();
    static void Destroy();

public:
	static ThreadManager* GetInstance();
};
