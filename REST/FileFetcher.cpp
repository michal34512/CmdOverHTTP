#include "FileFetcher.h"
#include <fstream>

std::wstring FileFetcher::GetExecutableDirectory()
{
    return executableDir;
}
void FileFetcher::SetExecutableDirectory(const wchar_t* _zeroArg) {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);

    std::wstring executablePath(buffer);

    size_t lastSlashPos = executablePath.find_last_of(L"\\/");
    if (lastSlashPos != std::wstring::npos) {
        executableDir = executablePath.substr(0, lastSlashPos);
    }
}
std::wstring FileFetcher::GetPassFilePatch()
{
    return executableDir + passFile;
}
std::wstring FileFetcher::GetPortFilePatch()
{
    return executableDir + portFile;
}
bool FileFetcher::FileExists(const std::wstring& filePath) {
    DWORD fileAttributes = GetFileAttributesW(filePath.c_str());

    return (fileAttributes != INVALID_FILE_ATTRIBUTES) && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

void FileFetcher::FetchPassAndPort()
{
    FetchPass();
    FetchPort();
}
void FileFetcher::FetchPass()
{
    passArgument = GetStringFromFile(GetPassFilePatch());
}
void FileFetcher::FetchPort()
{
    portArgument = GetStringFromFile(GetPortFilePatch());
}
std::string FileFetcher::GetStringFromFile(std::wstring _filePath)
{
    std::fstream file;
    file.open(_filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file.good()) return "";
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}
std::string FileFetcher::GetPass()
{
    return passArgument;
}
std::string FileFetcher::GetPort()
{
    return portArgument;
}