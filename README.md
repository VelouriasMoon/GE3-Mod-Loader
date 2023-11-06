# GE3 Mod Loader
 A plugin for God Eater 3 to enable Loading mods without deleting packs

### Requirements
[Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases), you'll want the one labeled "x64".

### Installation
- install the Ultimate ASI Loader dinput8.dll in the "God Eater 3" folder next to the exe
- if the plugins folder doesn't exist in your "God Eater 3" make one
- Add the GE3Plugin.asi and config.toml into your plugins folder

### Usage
The Mod loader contains many options in the config.toml in the plugins folder,
| Option | type | Info |
| -------- | ------ | -----|
| Enable_Console | bool | Enables showing the console |
| Remove_RandomSeed | bool | Removes the Random Seed on functions addresses |
| Enable_AQLLog | bool | Outputs AQL Log to the console |
| Enable_AQLFile | bool | Writes AQL log to a file |
| Enable_Mods | bool | Enables file redirect to mods folder | 
| ModsPath | string | Mods folder name to redirect too, must be in "GOD EATER 3" with the exe |
| DDSLoad | bool | Allows texture ".mds" calls to be redirected to ".dds" files instead |
| Advanced_Mod_Seeking | bool | Allows mod name subfolders and base game folder searching for redirect calls|

#### Notes about Advanced Mod Seeking:  
Advanced Mod Seeking Expands the places the plugin will search for replacement files, without it the plugin will only load from the mods folder structured like base game IE: `mods\resource`  
With Advanced Mod Seeking the plugin will search  
- `mods\resource`
- `resource`
- `mods\mod_name\resource`  
in that order stopping once it finds a file. Meaning the mods folder has the highest priority, then base game, then named mod sub folders. This may increase load times depending on PC and number of files.

### Credits
[DeathChaos25](https://github.com/DeathChaos25) - help setting up detours and toml 
