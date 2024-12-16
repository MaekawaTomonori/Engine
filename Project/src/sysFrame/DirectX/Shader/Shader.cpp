#include "Shader.h"

#include <cassert>
#include <format>

#include "System/System.h"

#pragma comment(lib, "dxcompiler.lib")

void Shader::CreateDxc() {
    HRESULT hr = S_OK;
    hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
    assert(SUCCEEDED(hr));
    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
    assert(SUCCEEDED(hr));

    hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
    assert(SUCCEEDED(hr));
}

void Shader::CompileShaders() {
    vertexShader_.Attach(Compile(L"Assets/Shaders/", L"Object3d.VS.hlsl", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get()));
    pixelShader_.Attach(Compile(L"Assets/Shaders/", L"Object3d.PS.hlsl", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get()));
}

bool Shader::Create() {
    CreateDxc();
    CompileShaders();

    return true;
}

IDxcBlob* Shader::Compile(const std::wstring& directoryPath, const std::wstring& filePath, const wchar_t* profile,
                          IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
    System::Log(System::ConvertString(std::format(L"Begin CompileShader, Path : {}, Profile : {}\n", filePath, profile)));
    IDxcBlobEncoding* shaderSource = nullptr;
    std::wstring fullPath = directoryPath + filePath;
    HRESULT hResult = dxcUtils->LoadFile(fullPath.c_str(), nullptr, &shaderSource);
    assert(SUCCEEDED(hResult));

    DxcBuffer shaderSourceBuffer;
    shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
    shaderSourceBuffer.Size = shaderSource->GetBufferSize();
    shaderSourceBuffer.Encoding = DXC_CP_UTF8;

    ///Compiling
    LPCWSTR arguments[] = {
        filePath.c_str(), //対象のhlslファイル名
        L"-E", L"main", //EntryPoint
        L"-T", profile, //ShaderProfile
        L"-Zi", L"-Qembed_debug", //DebugInfo
        L"-Od", //最適化を外す
        L"-Zpr", //メモリレイアウトは行優先
    };

    IDxcResult* shaderResult = nullptr;
    hResult = dxcCompiler->Compile(
        &shaderSourceBuffer,
        arguments,
        _countof(arguments),
        includeHandler,
        IID_PPV_ARGS(&shaderResult)
    );
    assert(SUCCEEDED(hResult));

    IDxcBlobUtf8* shaderError = nullptr;
    shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
    if (shaderError != nullptr && shaderError->GetStringLength() != 0){
        System::Log(shaderError->GetStringPointer());

        assert(false);
    }

    IDxcBlob* shaderBlob = nullptr;
    hResult = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
    assert(SUCCEEDED(hResult));

    System::Log(System::ConvertString(std::format(L"Compile Succeed, Path : {}, Profile : {}\n", filePath, profile)));
    shaderSource->Release();
    shaderResult->Release();

    return shaderBlob;
}
