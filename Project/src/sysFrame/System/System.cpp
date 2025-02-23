#include "System.h"

#include <windows.h>
#include <string>
#include <algorithm>
#include <rpc.h>

#pragma comment(lib, "rpcrt4.lib")

std::string System::CreateUuid() {
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR szUuid = nullptr;
    UuidToStringA(&uuid, &szUuid);
    struct UUIDCleaner{
        RPC_CSTR& ptr;
        ~UUIDCleaner() {
            if (ptr)RpcStringFreeA(&ptr);
        }
    } cleaner {szUuid};
    return reinterpret_cast<char*>(szUuid);
}

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

bool System::EqualsIgnoreCase(const std::string& str1, const std::string& str2) {
    return std::ranges::equal(str1, str2, [](const char c1, const char c2){
        return std::tolower(c1) == std::tolower(c2);
    });
}

void System::Log(Log::Level level, const std::string& message) {
	class Log* logging = Log::GetLogger();
    if (level == Log::Level::INFO){logging->Info(message); return;}
    if (level == Log::Level::DEBUG){logging->Debug(message); return;}
    if (level == Log::Level::WARN){logging->Warning(message); return;}
    if (level == Log::Level::ERR){logging->Error(message); return;}
    //OutputDebugStringA(message.c_str());
    
}

void System::Log(Log::Level level, const std::wstring& message) {
    std::string msg = ConvertString(message);
    class Log* logging = Log::GetLogger();
    if (level == Log::Level::INFO){logging->Info(msg); return;}
    if (level == Log::Level::DEBUG){logging->Debug(msg); return;}
    if (level == Log::Level::WARN){logging->Warning(msg); return;}
    if (level == Log::Level::ERR){logging->Error(msg); return;}
    //OutputDebugStringA(ConvertString(message).c_str());
	
}

void System::Log(const std::string& message) {
    Log(Log::Level::DEBUG, message);
}

void System::Log(const std::wstring& message) {
    Log(Log::Level::DEBUG, message);
}
