#pragma once
#include <memory>
#include <spdlog/logger.h>

class Logger{
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

	Logger() = default;
	~Logger() = default;

	static Logger* instance_;
	static std::once_flag onceFlag_;
public:
	static Logger* GetLogger();
	static void Create();
	static void Destroy();

	void Initialize();

	void Info(const std::string& msg) const;
	void Debug(const std::string& msg) const;
	void Warning(const std::string& msg) const;
    void Error(const std::string& msg) const;
};

