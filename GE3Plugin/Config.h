#pragma once

class config
{
public:
	static bool enableConsole;
	static bool enableAQLLog;
	static bool enableAQLFile;
	static bool enableMods;
	static bool enableDDSLoad;
	static std::string ModsPath;

	static bool init();
};