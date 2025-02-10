#pragma once
#include <string>

#include "Log/Log.h"

namespace System{
    std::wstring ConvertString(const std::string& str);

    std::string ConvertString(const std::wstring& str);

    bool EqualsIgnoreCase(const std::string& str1, const std::string& str2);

    void Log(Log::Level level, const std::string& message);

    void Log(Log::Level level, const std::wstring& message);

    void Log(const std::string& message);

    void Log(const std::wstring& message);
};

