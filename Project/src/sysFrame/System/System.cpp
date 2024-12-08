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
    if (level == Log::Level::INFO){Log::Info(message); return;}
    if (level == Log::Level::DEBUG){Log::Debug(message); return;}
    if (level == Log::Level::WARN){Log::Warning(message); return;}
    if (level == Log::Level::ERR){Log::Error(message); return;}
    //OutputDebugStringA(message.c_str());
}

void System::Log(Log::Level level, const std::wstring& message) {
    std::string msg = ConvertString(message);
    if (level == Log::Level::INFO){Log::Info(msg); return;}
    if (level == Log::Level::DEBUG){Log::Debug(msg); return;}
    if (level == Log::Level::WARN){Log::Warning(msg); return;}
    if (level == Log::Level::ERR){Log::Error(msg); return;}
    //OutputDebugStringA(ConvertString(message).c_str());
}

void System::Log(const std::string& message) {
    Log(Log::Level::DEBUG, message);
}

void System::Log(const std::wstring& message) {
    Log(Log::Level::DEBUG, message);
}
