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

void System::Log(const std::string& message) {
    OutputDebugStringA(message.c_str());
}

void System::Log(const std::wstring& message) {
    OutputDebugStringA(ConvertString(message).c_str());
}