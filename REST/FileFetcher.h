#pragma once
#include <iostream>
#include <windows.h>

class FileFetcher
{
    std::wstring executableDir = L"";
    const std::wstring passFile = L"\\pass.txt";
    const std::wstring portFile = L"\\port.txt";
    
    std::string passArgument = "";
    std::string portArgument = "";

    std::string GetStringFromFile(std::wstring _filePath);
    void FetchPass();
    void FetchPort();
public:
    std::wstring GetExecutableDirectory();
    void SetExecutableDirectory(const wchar_t* _zeroArg);
    std::wstring GetPassFilePatch();
    std::wstring GetPortFilePatch();
    bool FileExists(const std::wstring& filePath);
    void FetchPassAndPort();
    
    std::string GetPort();
    std::string GetPass();
};