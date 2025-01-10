#include "Logger.h"

#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/sinks/ostream_sink.h"

Logger* Logger::instance_ = nullptr;
std::once_flag Logger::onceFlag_;

Logger* Logger::GetLogger() {
    call_once(onceFlag_, Create);
    assert(instance_);
    return instance_;
}

void Logger::Create() {
    instance_ = new Logger();
    instance_->Initialize();
    //SingletonFinalizer::AddFinalizer(&Destroy);
    instance_->Info("Logger Enabled");
}

void Logger::Destroy() {
    delete instance_;
    instance_ = nullptr;
}

void Logger::Initialize() {
    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/latest.log", true);
    auto msvc = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    auto console = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout);

    sinks_ = {file, msvc, console};
    combined_logger = std::make_shared<spdlog::logger>("logger", sinks_.begin(), sinks_.end());
	set_default_logger(combined_logger);

    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%D-%R:%S.%e%f%F][Thread:%t][%l]:%v");

    spdlog::info("LogSystem Enabled");
}

void Logger::Info(const std::string& msg) const {
    combined_logger->info(msg);
}

void Logger::Debug(const std::string& msg) const {
    combined_logger->debug(msg);
}

void Logger::Warning(const std::string& msg) const {
    combined_logger->warn(msg);
}

void Logger::Error(const std::string& msg) const {
    combined_logger->error(msg);
}
