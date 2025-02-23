#include "File.h"

#include <iostream>
#include <Windows.h>

bool File::Compress(const std::string& src, const std::string& dst) {
    HANDLE hInputFile = CreateFileA(
        src.c_str(),
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hInputFile == INVALID_HANDLE_VALUE){
        std::cerr << "Failed to open input file: " << GetLastError() << std::endl;
        return false;
    }

    HANDLE hOutputFile = CreateFileA(
        dst.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hOutputFile == INVALID_HANDLE_VALUE){
        std::cerr << "Failed to create output file: " << GetLastError() << std::endl;
        CloseHandle(hInputFile);
        return false;
    }

    // 圧縮オプションを設定
    DWORD compressionType = COMPRESSION_FORMAT_DEFAULT;
    DWORD bytesReturned = 0;
    if (!DeviceIoControl(
        hOutputFile,
        FSCTL_SET_COMPRESSION,
        &compressionType,
        sizeof(compressionType),
        nullptr,
        0,
        &bytesReturned,
        nullptr)){
        std::cerr << "Failed to enable compression: " << GetLastError() << std::endl;
        CloseHandle(hInputFile);
        CloseHandle(hOutputFile);
        return false;
    }

    // 入力ファイルから出力ファイルへのコピー
    char buffer[4096];
    DWORD bytesRead, bytesWritten;

    while (ReadFile(hInputFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0){
        if (!WriteFile(hOutputFile, buffer, bytesRead, &bytesWritten, nullptr) || bytesWritten != bytesRead){
            std::cerr << "Failed to write to compressed file: " << GetLastError() << std::endl;
            CloseHandle(hInputFile);
            CloseHandle(hOutputFile);
            return false;
        }
    }

    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);

    std::cout << "File successfully compressed and saved as " << dst << std::endl;
    return true;
}
