# ***P2SourceModPlusPlus*** (Formerly ***WorkshopStopper9000***)

## ***A Source Engine Plugin for Portal 2 Source Mods that fixes and adds some QoL features for them!***

> [!INFO]
> I originally planned `WorkshopStopper9000` to fix the workshop downloading issue, but I hope to add more features and fixes, so now it has become `P2SourceModPlusPlus`. I hope to implement some good features the community will use until the fated day Portal 2: Community Edition is released. This plugin will act as a temporary solution for mod makers. Once P2:CE's public beta and/or release occurs, I will probably not work on this anymore, as most of the stuff here is fixed over there.

> [!WARNING]
> This plugin was designed in mind for SourceMods, meaning mods loaded through the `sourcemods` folder in Steam. Not for Portal 2 mods that are on Steam.
>
> While this plugin should be ok for a Steam Portal 2 mod, note that:
> 1. I do not know if Valve will allow such modifications on the Steam platform, especially if it involves one of their games.
> 2. Compatibility will not be 100%, so expect bugs or other issues.
> 3. If you are using this plugin with your mod, I am not responsible if anything bad happens to your mod's place on Steam.
>
> I will, in the future, try to add custom elements involving interactions with Steam's API, like achievements, but for now, I don't expect this to work correctly with Steam mods. Plus, in the end, P2:CE will do this properly in the future.

### Features Implemented/Plan to Implement:

* Stopped workshop maps from being downloaded by the game because the workshop folder isn't on path for Source Mods. Because of this, it tries to get ***ALL*** of them at once, which causes the game to be unstable and occasionally crash.
* Fixed human NPC movement so they don't jitter when they move. `cl_localnetworkbackdoor` is automatically set to 0 without manual adjustment.
* Made `r_screenoverlay` ConVar be enabled by default and not need cheats so `env_screenoverlay` entities can work correctly without `sv_cheats` being needed.
* **(NOT IMPLEMENTED)** Option to turn on or off puzzle maker functionality. Most mods don't use the puzzle maker, but it still loads in. The plugin will try to disable as much of it as possible so the console is not filled with any puzzle-maker-related errors, messages, or other additional bloat.
* **(NOT IMPLEMENTED)** Custom Discord RPC for Portal 2 Source Mods.
* **(NOT IMPLEMENTED)** Additional achievement support and features. Like custom achievements that span multiple maps and have more parts than Door Prize and Smash TV.
* **(NOT IMPLEMENTED)** Use of player flashlight without needing cheats enabled.
* **(NOT IMPLEMENTED)** As informed by MarvinG, `env_sun` disappears when over player crosshair. I have not confirmed this, and have never noticed it in general.

### Download and Install Instructions:

Download the latest `addons.zip` under the `Releases` tab on GitHub and extract its folder into the base directory of your Source Mod. Portal 2 should automatically start the plugin up with the game and thus stop the game from downloading workshop maps. You should be able to see a message in the developer console from the plugin if it loaded. That's it. Please create an Issue post on the plugin's repository if there are any issues. I'll get to it within the next 20 years. (Hopefully not).

**As of writing, 11/10/2024, the plugin is only compatible with Windows. The signature scanner it uses still needs to be compatible with Linux, plus overall, the way the P2:MM plugin is structured (this plugin is built off of some of P2:MM plugin code as a base), it's impossible to compile for Linux anyway. Literally 1984, I know, but I don't have enough time or experience to figure it out by myself. Linux users should still be able to get away with using Proton or similar to be able to use the plugin's Windows `.dll`. Hopefully, one day, this can be fixed...**

**Update 3/24/2025: I plan to move this over to a forked version of SAR using its code base as a base for this plugin. Its structure is much better, it has more engine functionality accessible in the plugin, and it compiles for Linux. However, do not expect this switch to happen anytime soon. Thank you for being so patient.**

***
Please credit Orsell/OrsellGaming, Nanoman2525, and NULLderef, or the `Portal 2: Multiplayer Mod Team` if you use this plugin or its code in any way with your Source Mod.
You also need to include this repository and P2:MM's license file with the plugin.
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
