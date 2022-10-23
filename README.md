# GE3 Mod Loader
 A plugin for God Eater 3 to enable Loading mods without deleting packs

### Requirements
[Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases), you'll want the one labeled "x64".

### Installation
- install the Ultimate ASI Loader dinput8.dll in the "God Eater 3" folder next to the exe
- extract the Mod loader into the same folder

### Usage
The Mod loader contains many options in the config.toml in the plugins folder,
| Option | type | Info |
| -------- | ------ | -----|
| Enable_Console | bool | Enables showing the console |
| Enable_AQLLog | bool | Outputs AQL Log to the console |
| Enable_AQLFile | bool | Writes AQL log to a file |
| Enable_Mods | bool | Enables file redirect to mods folder | 
| ModsPath | string | Mods folder name to redirect too, must be in "GOD EATER 3" with the exe |
| DDSLoad | bool | Allows texture ".mds" calls to be redirected to ".dds" files instead |
### Credits
[DeathChaos25](https://github.com/DeathChaos25) - help setting up detours and toml 