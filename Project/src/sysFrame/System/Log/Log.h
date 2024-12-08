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
	static std::vector<spdlog::sink_ptr> sinks_;
	static std::shared_ptr<spdlog::logger> combined_logger;

public:
	~Log(){}
	static void Initialize();

	static void Info(const std::string& msg);
	static void Debug(const std::string& msg);
	static void Warning(const std::string& msg);
    static void Error(const std::string& msg);
};

