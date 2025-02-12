#include "Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"
#include "System/SingletonFinalizer/SingletonFinalizer.h"

Log* Log::instance_ = nullptr;
std::once_flag Log::onceFlag_;

Log* Log::GetLogger() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void Log::Create() {
    instance_ = new Log();
    instance_->Initialize();
    SingletonFinalizer::AddFinalizer(&Destroy);
    instance_->Info("Logger Enabled");
}

void Log::Destroy() {
    delete instance_;
    instance_ = nullptr;
}

void Log::Initialize() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::floor<std::chrono::seconds>(now);
    std::string date = std::format("{:%Y.%m.%d_%H.%M.%S}", now_time);

    path_ = "logs/" + date + ".log";

    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path_, true);
    auto console = std::make_shared<spdlog::sinks::msvc_sink_mt>();

    sinks_ = {file, console};
    combined_logger = std::make_shared<spdlog::logger>("logger", sinks_.begin(), sinks_.end());
	set_default_logger(combined_logger);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%D-%R][Thread:%t][%l]:%v");
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
