#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"

std::shared_ptr<Log> Log::instance_ = nullptr;

std::shared_ptr<Log> Log::GetLogger() {
    if(!instance_){
        instance_ = std::shared_ptr<Log>(new Log, [](const Log* ptr){
            delete ptr;
        });
    }

    return instance_;
}

void Log::Initialize() {
    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/latest.log", true);
    auto console = std::make_shared<spdlog::sinks::msvc_sink_mt>();

    sinks_ = {file, console};
    combined_logger = std::make_shared<spdlog::logger>("logger", sinks_.begin(), sinks_.end());
	set_default_logger(combined_logger);

    spdlog::set_pattern("[%D-%R][Thread:%t] [%l]: %v");

    spdlog::info("LogSystem Enabled");
}

void Log::Info(const std::string& msg) const {
    combined_logger->info(msg);
}

void Log::Debug(const std::string& msg) const {
    combined_logger->debug(msg);
}

void Log::Warning(const std::string& msg) const {
    combined_logger->warn(msg);
}

void Log::Error(const std::string& msg) const {
    combined_logger->error(msg);
}
