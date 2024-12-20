#pragma once
#include <string>

#include "Log/Log.h"

namespace System{
    std::wstring ConvertString(const std::string& str);

    std::string ConvertString(const std::wstring& str);

    void Log(Log::Level level, const std::string& message);

    void Log(Log::Level level, const std::wstring& message);

    void Log(const std::string& message);

    void Log(const std::wstring& message);
};

