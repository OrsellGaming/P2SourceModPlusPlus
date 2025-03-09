# ***P2SourceModPlusPlus*** (Formerly ***WorkshopStopper9000***)

## ***A Source Engine Plugin for Portal 2 Source Mods that fixes and adds some QoL features for them!***

> I originally planned `WorkshopStopper9000` to simply fix the workshop downloading issue, but I hope to add a bit more features and fixes so now it has become `P2SourceModPlusPlus`. Hopefully I can implement some good features that will be used by the community until the fated day Portal 2: Community Edition is released. Basically this is gonna be a temporarily solution for mod makers. Once P2:CE's public beta and/or release occurs, I will most likely not work on this anymore as most of the stuff here is fixed over there.

> [!WARNING]
> This plugin was designed in mind for SourceMods, meaning mods loaded through the `sourcemods` folder in Steam. Not for Portal 2 mods that are on Steam.
>
> While this plugin should be fine for a Steam Portal 2 mod, note that:
> 1. I do not know if Valve will allows such modifications on the Steam platform, especially if it's involving one of their games.
> 2. Compatibility will not be 100% so expect bugs or other issues.
> 3. I am not responsible if anything bad happens to your mod's place on Steam if you are using this plugin with your mod.
>
> I will in the future try and add custom elements that will involve interactions with Steam's API, like achievements, but for now don't expect this to properly work with Steam mods. Plus again, in the end, P2:CE will do this properly in the future.

### Features Implemented/Plan to Implement:

* Stopped workshop maps from being downloaded by the game because the workshop folder isn't on path for Source Mods. Because of this it tries to get ***ALL*** of them at once, which caused the game to be unstable and on occasion crash.
* Fix human NPC movement so they don't jitter when they move.
* **(NOT IMPLEMENTED)** Option to turn on or off Puzzlemaker functionality. Most mods don't use the puzzlemaker, but it still loads in. The plugin will try to disable as much of it as possible so console is not filled with any puzzlemaker related errors or messages or any other additional bloat.
* **(NOT IMPLEMENTED)** Custom Discord RPC for Portal 2 Source Mods.
* **(NOT IMPLEMENTED)** Additional achievement support and features. Like custom achievements that span multiple maps that have more parts than Door Prize and Smash TV.
* **(NOT IMPLEMENTED)** Use of player flashlight without needing cheats enabled.
* **(NOT IMPLEMENTED)** By default have env_screenoverlay entities not need the `r_screenoverlay` ConVar be enabled.

### Download and Install Instructions:

Download the latest `addons.zip` under the `Releases` tab on GitHub and extract its folder into the base directory of your Source Mod. Portal 2 should automatically start the plugin up with the game and thus stop the game from downloading workshop maps. You should be able to see a message in the developer console from the plugin if it loaded. That's it. This took about 20 minutes to make, so please create an Issue post on the plugin's repository if there are any issues. I'll get to it within the next 20 years.

**As of writing, 11/10/2024, the plugin is only compatible with Windows. The signature scanner it uses still needs to be compatible with Linux, plus overall, the way the P2:MM plugin is structured (this plugin is built off of some of P2:MM plugin code as a base), it's impossible to compile for Linux anyway. Literally 1984, I know, but I don't have enough time or experience to figure it out by myself. Linux users should still be able to get away with using Proton or similar to be able to use the plugin's Windows `.dll`. Hopefully, one day, this can be fixed...**

***
Please credit either Orsell/OrsellGaming, Nanoman2525, and NULLderef, or the `Portal 2: Multiplayer Mod Team` if you use this plugin or its code in any way with your Source Mod.
You must also include this repository and P2:MM's license file with the plugin.
Both licenses will be included with the `addon.zip` file you download to get the plugin.
***

```c++
MIT License

Copyright (c) 2024 Portal 2: Multiplayer Mod Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
