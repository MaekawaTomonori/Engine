#pragma once
#include <string>

#include "Log/Logger.h"

namespace System{
    std::wstring ConvertString(const std::string& str);

    std::string ConvertString(const std::wstring& str);

    void Log(Logger::Level level, const std::string& message);

    void Log(Logger::Level level, const std::wstring& message);

    void Log(const std::string& message);

    void Log(const std::wstring& message);
};

