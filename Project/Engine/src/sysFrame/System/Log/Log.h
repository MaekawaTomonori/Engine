#pragma once
#include <memory>
#include <spdlog/logger.h>

class Log{
public:
	enum class Level{
		TRACE,
		INFO,
		DEBUG,
		WARN,
        ERR,
		FATAL
	};

private:
	std::vector<spdlog::sink_ptr> sinks_;
	std::shared_ptr<spdlog::logger> combined_logger;

	Log() = default;
	~Log() = default;

	static Log* instance_;
	static std::once_flag onceFlag_;
public:
	static Log* GetLogger();
	static void Create();
	static void Destroy();

	void Initialize();

	void Info(const std::string& msg) const;
	void Debug(const std::string& msg) const;
	void Warning(const std::string& msg) const;
    void Error(const std::string& msg) const;
};

