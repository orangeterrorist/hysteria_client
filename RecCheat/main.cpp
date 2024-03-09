#include <windows.h>
#include <iostream>
#include <Minhook.h>
#include <random>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>

//auth
#include <curl/curl.h>
#include "main.h"
#include <Lmcons.h>
#include <iomanip>
#include <cstring>
#include "json.hpp"


// fix notif vuln error
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

// Offsets below

uintptr_t Hysteria = 0x2E024B0; // bool true IsValid (SafeReportResult, Hysteria) get from DnSpy uhhhhhhhhhhhhhggggggg

void showNotification(const std::string& message, int timeout) {
    const std::string VerNum{ "v1" };
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(nid);
    nid.hWnd = NULL; // handle to the window that will receive notifications
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 9999999999999999999);
    nid.uID = dist(rng); // set nid.uID to a random value
    nid.uFlags = NIF_ICON | NIF_INFO;
    nid.hIcon = LoadIcon(NULL, IDI_EXCLAMATION); // load the exclamation icon
    nid.uTimeout = timeout;
    nid.dwInfoFlags = NIIF_INFO;
    std::string full_title = "Hysetria " + VerNum + "\nby neptune#1995";  // title of the notification
    strcpy_s(nid.szInfoTitle, full_title.c_str());
    strcpy_s(nid.szInfo, std::string(message).c_str()); // message to display in the notification
    Shell_NotifyIcon(NIM_ADD, &nid); // show the notification
}

bool(__fastcall* utrue_o)(DWORD*);
bool __stdcall utrue_hook(DWORD* uh) {
    return true;
}

void init()
{
    // don't make this anymore cancer than it needs to be, whole init func needs to be rewritten
        const std::string VerNum{ "v1" };
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria), &utrue_hook, (LPVOID*)&utrue_o);

        std::cout << "Hysetria Client " + VerNum + " Initialized Successfully! \nHello Rec Room! >:^)" << std::endl;
        showNotification("Hysetria Client has been initialized!", 2000);
}

// thing for auth
size_t writeCallback(char* buf, size_t size, size_t nmemb, std::string* response) {
    response->append(buf, size * nmemb);
    return size * nmemb;
}

void consoleThread() {
    const std::string VerNum{ "v1" };
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    std::string title = "Hysetria Client | by neptune#1995";
    SetConsoleTitleA(title.c_str());

    bool exit{ false };
    bool toggle{ false };

    while (true)
    {
        std::string cmd;
        std::cout << " > ";
        std::getline(std::cin, cmd);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { return std::tolower(c); });

        if (cmd == "toggle")
        {
            toggle = !toggle;

            if (toggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria));
                std::cout << "[Hysetria] Toggled Hysetria On." << std::endl;
                showNotification("Hysetria Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria));
                std::cout << "[Hysetria] Toggled Hysetria Off." << std::endl;
                showNotification("Hysetria Toggled Off.", 500);
            }
        }
        else if (cmd == "quit")
        {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            std::cout << "[Hysetria] Unloading DLL, you can now close the console." << std::endl;
            showNotification("Hysetria Client has Unloaded.", 1000);
            break;
        }
        else if (cmd == "exit")
        {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            exit = true;
            std::cout << "[Hysetria] Exiting." << std::endl;
            showNotification("Hysetria Client Exited.", 1000);
            break;
        }
        else if (cmd == "help")
        {
            std::cout << "- toggle: OWW MY EARS FUCK YOU LUCID" << std::endl;
            std::cout << "- quit: Unloads Hysetria" << std::endl;
            std::cout << "- exit: Unloads Hysetria & quits Rec Room" << std::endl;
            std::cout << "- help: shake end die" << std::endl;
        }
        else
        {
        }
        std::cin.clear();
    }
    FreeConsole();
    DLL_PROCESS_DETACH;
    if (exit)
    {
        std::exit(80085);
    }
    ExitThread;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::thread(consoleThread).detach();
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    switch (ul_reason_for_call)
        return TRUE;
}