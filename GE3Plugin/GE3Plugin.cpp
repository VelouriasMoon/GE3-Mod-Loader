// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include "Signature.h"
#include "Config.h"
#include "structs.h"

#pragma region Functions

void WriteToLog(const std::string& text)
{
    std::ofstream log_file("plugins\\AQLLog.log", std::ios_base::out | std::ios_base::app);
    log_file << text;// << std::endl;
}

bool FileExists(char* path)
{
    FILE* file;
    if (file = fopen(path, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

void SimpleFileLoader(char* filename)
{
    char* finalpath = new char[0x100];
    strcpy(finalpath, config::ModsPath.c_str());
    strcat(finalpath, "/");
    strcat(finalpath, filename);
    //printf("[Debug Logger] modspath %s\r\n", finalpath);

    if (FileExists(finalpath))
    {
        memcpy(filename, finalpath, strlen(finalpath) + 1);
        printf("[ModLoader] Replaced file call %s\r\n", finalpath);
    }
    else if (config::enableDDSLoad)
    {
        std::string s(finalpath);
        std::string mds = ".mds";
        std::string dds = ".dds";

        std::size_t index;
        while ((index = s.find(mds)) != std::string::npos)
            s.replace(index, dds.length(), dds);

        strcpy(finalpath, s.c_str());

        if (FileExists(finalpath))
        {
            memcpy(filename, finalpath, strlen(finalpath) + 1);
            printf("[ModLoader] Replaced file call %s\r\n", finalpath);
        }
    }
    return;
}

void AdvanceFileLoader(char* filename)
{
    char* finalpath = new char[0x100];
    strcpy(finalpath, config::ModsPath.c_str());
    strcat(finalpath, "\\*");

    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = FindFirstFileA(finalpath, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("[GE3PluginLog] Failed to find files in Mods directory.\r\n");
        return;
    }

    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            strcpy(finalpath, config::ModsPath.c_str());
            strcat(finalpath, "/");
            strcat(finalpath, FindFileData.cFileName);
            strcat(finalpath, "/");
            strcat(finalpath, filename);
            //printf("[Debug Logger] modspath %s\r\n", finalpath);

            if (FileExists(finalpath))
            {
                memcpy(filename, finalpath, strlen(finalpath) + 1);
                printf("[ModLoader] Replaced file call %s\r\n", finalpath);
                break;
            }
            else if (config::enableDDSLoad)
            {
                std::string s(finalpath);
                std::string mds = ".mds";
                std::string dds = ".dds";

                std::size_t index;
                while ((index = s.find(mds)) != std::string::npos)
                    s.replace(index, dds.length(), dds);

                strcpy(finalpath, s.c_str());

                if (FileExists(finalpath))
                {
                    memcpy(filename, finalpath, strlen(finalpath) + 1);
                    printf("[ModLoader] Replaced file call %s\r\n", finalpath);
                    break;
                }
            }
        }
    } while (FindNextFileA(hFind, &FindFileData) != 0);
    FindClose(hFind);
    return;
}

#pragma endregion

#pragma region Sigs

void* LoadPack = sigScan(
    "\x48\x8B\xC4\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x81\xEC\x80\x00\x00\x00\x48\xC7\x45\x2A\xFE\xFF\xFF\xFF\x48\x89\x58\x2A\x48\x89\x70\x2A\x48\x89\x78\x2A\x48\x8B\x05\x2A\x2A\x2A\x2A\x48\x33\xC4\x48\x89\x45\x2A\x48\x8B\xD9",
    "xxxxxxxxxxxxxxxxxxxxx?xxxxxxx?xxx?xxx?xxx????xxxxxx?xxx");

void* CreateFileGE3 = sigScan(
    "\x48\x89\x5C\x24\x2A\x48\x89\x6C\x24\x2A\x48\x89\x74\x24\x2A\x57\x48\x81\xEC\x60\x04\x00\x00",
    "xxxx?xxxx?xxxx?xxxxxxxx");

//0x140c93060
void* AQLLog = sigScan(
    "\x40\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x2A\xFE\xFF\xFF\xFF\x48\x89\x5C\x24\x2A\x48\x89\x74\x24\x2A\x48\x8B\xD9\x48\x8D\x35\x2A\x2A\x2A\x2A",
    "xxxxxxxxxx?xxxxxxxx?xxxx?xxxxxx????");

//0x140d75510
void* LoaderFunction = sigScan(
    "\x48\x8B\xC4\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\xA8\x2A\x2A\x2A\x2A\x48\x81\xEC\x90\x02\x00\x00\x48\xC7\x44\x24\x2A\xFE\xFF\xFF\xFF\x48\x89\x58\x2A\x48\x89\x70\x2A\x48\x8B\x05\x2A\x2A\x2A\x2A\x48\x33\xC4\x48\x89\x85\x2A\x2A\x2A\x2A\x4C\x8B\xFA",
    "xxxxxxxxxxxxxx????xxxxxxxxxxx?xxxxxxx?xxx?xxx????xxxxxx????xxx");

//0x140c72b40
void* SetRandomSeed = sigScan(
    "\xB8\xFF\x7F\x00\x00\x3B\xC8",
    "xxxxxxx");

#pragma endregion

#pragma region Hooks

HOOK(void, __stdcall, hook_PackRead, LoadPack, uint16_t* param1)
{
    printf("FunctionTriggerLog: PackRead?(%p)\n", param1);
    return orig_hook_PackRead(param1);
}

HOOK(void, __stdcall, hook_CreateFileGE3, CreateFileGE3, LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE  hTemplateFile)
{
    printf("[File Access Log] %hS\r\n", lpFileName);
    return orig_hook_CreateFileGE3(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HOOK(void, __stdcall, hook_AQLLog, AQLLog, char* str)
{
    if (config::enableAQLLog)
        printf("[AQL Log] %hS", str);
    if(config::enableAQLFile)
        WriteToLog(str);
    return orig_hook_AQLLog(str);
}

HOOK(HANDLE, __stdcall, hook_CreateFileA, &CreateFileA, LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE  hTemplateFile)
{
    if (!strstr(lpFileName, "hid#")) {
        printf("[File Logger] %s\r\n", lpFileName);
    }
    return orig_hook_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

HOOK(void, __stdcall, hook_LoaderFunc, LoaderFunction, uint64_t* param1, uint64_t* param2)
{
    char* filename = (char*)((uint64_t)param2 + 0xe);

    if (config::ModSubFolders)
        AdvanceFileLoader(filename);
    else
        SimpleFileLoader(filename);
    
    return orig_hook_LoaderFunc(param1, param2);
}

HOOK(void, __stdcall, hook_SetRandomSeed, SetRandomSeed, u32 seed) 
{
    printf("[GE3PluginLog] Replacing Random Seed with 0\r\n");
    return orig_hook_SetRandomSeed(0);
}

#pragma endregion

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        config::init();
        if (!GetConsoleWindow() && config::enableConsole) // open console output
        {
            AllocConsole();
            AttachConsole(GetCurrentProcessId());
            freopen("CON", "w", stdout);
        }

        if (AQLLog && (config::enableAQLLog || config::enableAQLFile)) {
            if (std::filesystem::exists("..\\plugins\\AQLLog.log"))
            {
                std::filesystem::remove("..\\plugins\\AQLLog.log");
            }
            printf("[SigScan Logger] 0x%p : AQL Log\r\n", AQLLog);
            INSTALL_HOOK(hook_AQLLog);
        }

        if (LoaderFunction && config::enableMods) {
            char* filepath = new char[0x50];
            strcpy(filepath, "../");
            strcat(filepath, config::ModsPath.c_str());
            DWORD dwAttrib = GetFileAttributesA(filepath);

            if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
                printf("[SigScan Logger] 0x%p : Loader::LoadFile\r\n", LoaderFunction);
                INSTALL_HOOK(hook_LoaderFunc);
            }
            else
            {
                config::ModsPath = "plugins";
                printf("[GE3PluginLog] Failed to find files in Mods directory, defaulting to plugins folder\r\n");
                printf("[SigScan Logger] 0x%p : Loader::LoadFile\r\n", LoaderFunction);
                INSTALL_HOOK(hook_LoaderFunc);
            }
        }

        if (SetRandomSeed && config::RemoveRandomSeed) {
            printf("[SigScan Logger] 0x%9 : ASLR::SetRandomSeend\r\n", SetRandomSeed);
            INSTALL_HOOK(hook_SetRandomSeed)
        }
        

        return TRUE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
