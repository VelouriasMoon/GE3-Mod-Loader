#pragma once

#include "structs.h"

class config
{
public:
	static bool enableConsole;
	static bool enableAQLLog;
	static bool enableAQLFile;
	static bool enableMods;
	static bool enableDDSLoad;
	static std::string ModsPath;
	static bool RemoveRandomSeed;
	static bool ModSubFolders;

	static bool init();
};