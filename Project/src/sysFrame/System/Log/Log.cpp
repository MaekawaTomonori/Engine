#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"

std::vector<spdlog::sink_ptr> Log::sinks_;
std::shared_ptr<spdlog::logger> Log::combined_logger;

void Log::Initialize() {
    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/latest.log", true);
    auto console = std::make_shared<spdlog::sinks::msvc_sink_mt>();

    sinks_ = {file, console};
    combined_logger = std::make_shared<spdlog::logger>("logger", sinks_.begin(), sinks_.end());
	set_default_logger(combined_logger);

    spdlog::set_pattern("[%D-%R][Thread:%t] [%l]: %v");

    spdlog::info("LogSystem Enabled");
}

void Log::Info(const std::string& msg) {
    spdlog::info(msg);
}

void Log::Debug(const std::string& msg) {
    spdlog::debug(msg);
}

void Log::Warning(const std::string& msg) {
    spdlog::warn(msg);
}

void Log::Error(const std::string& msg) {
    spdlog::error(msg);
}
