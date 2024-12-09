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

	static std::shared_ptr<Log> instance_;
public:
	static std::shared_ptr<Log> GetLogger();

	void Initialize();

	void Info(const std::string& msg) const;
	void Debug(const std::string& msg) const;
	void Warning(const std::string& msg) const;
    void Error(const std::string& msg) const;
};

