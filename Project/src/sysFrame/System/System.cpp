#include "System.h"

#include <windows.h>

std::wstring System::ConvertString(const std::string& str) {
    if (str.empty()){
        return {};
    }

    auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.size()), nullptr, 0);
    if (sizeNeeded == 0){
        return {};
    }
    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.size()), result.data(), sizeNeeded);
    return result;
}

std::string System::ConvertString(const std::wstring& str) {
    if (str.empty()){
        return {};
    }

    auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);
    if (sizeNeeded == 0){
        return {};
    }
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, nullptr, nullptr);
    return result;
}

void System::Log(Log::Level level, const std::string& message) {
	std::weak_ptr<class Log> w = Log::GetLogger();
    if (std::shared_ptr<class Log> logging = w.lock()){
	    if (level == Log::Level::INFO){logging->Info(message); return;}
	    if (level == Log::Level::DEBUG){logging->Debug(message); return;}
	    if (level == Log::Level::WARN){logging->Warning(message); return;}
	    if (level == Log::Level::ERR){logging->Error(message); return;}
    //OutputDebugStringA(message.c_str());
    }
}

void System::Log(Log::Level level, const std::wstring& message) {
    std::string msg = ConvertString(message);
    std::weak_ptr<class Log> w = Log::GetLogger();
    if(std::shared_ptr<class Log> log = w.lock()){
	    if (level == Log::Level::INFO){log->Info(msg); return;}
	    if (level == Log::Level::DEBUG){log->Debug(msg); return;}
	    if (level == Log::Level::WARN){log->Warning(msg); return;}
	    if (level == Log::Level::ERR){log->Error(msg); return;}
	    //OutputDebugStringA(ConvertString(message).c_str());
	}
}

void System::Log(const std::string& message) {
    Log(Log::Level::DEBUG, message);
}

void System::Log(const std::wstring& message) {
    Log(Log::Level::DEBUG, message);
}
