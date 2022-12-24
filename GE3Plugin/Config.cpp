#include "pch.h"
#include "Config.h"

bool config::enableConsole;
bool config::enableAQLLog;
bool config::enableAQLFile;
bool config::enableMods;
bool config::enableDDSLoad;
std::string config::ModsPath;
bool config::RemoveRandomSeed;

bool config::init()
{
    toml::table config;
    std::string fileString = "";

    try
    {
        std::ifstream file("config.toml");
        std::string str;
        while (std::getline(file, str))
            fileString += str + "\n";

        config = toml::parse(fileString);
    }
    catch (std::exception& exception)
    {
        char text[1024];
        sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
        MessageBoxA(nullptr, text, "File Access Log", MB_OK | MB_ICONERROR);
    }

    enableConsole = config["General"]["Enable_Console"].value_or(true);
    RemoveRandomSeed = config["General"]["Remove_RandomSeed"].value_or(true);

    enableAQLLog = config["AQLLog"]["Enable_AQLLog"].value_or(false);
    enableAQLFile = config["AQLLog"]["Enable_AQLFile"].value_or(false);

    enableMods = config["ModLoader"]["Enable_Mods"].value_or(false);
    ModsPath = config["ModLoader"]["ModsPath"].value_or("mods");
    enableDDSLoad = config["ModLoader"]["DDSLoad"].value_or(false);

    return true;
}