# ***P2SourceModPlusPlus*** (Formerly ***WorkshopStopper9000***)

## ***A Source Engine Plugin for Portal 2 SourceMods that fixes and adds some QoL features for them!***

> [CAUTION]
> This has been a repeating pattern where I fix the plugin and it breaks itself later for no reason.
> Please just use version `1.0` until this plugin has been more optimized and cleanly implemented.

> [!NOTE]
> I originally planned `WorkshopStopper9000` to fix the workshop downloading issue, but I hope to add more features and fixes, so now it has become `P2SourceModPlusPlus`. I plan to implement some good features the community will use until the fated day Portal 2: Community Edition (P2:CE) is released. This plugin will act as a temporary solution for mod makers. Once P2:CE's public beta and/or release occurs, I will probably not work on this anymore, as most of the stuff here is fixed over there.

> [!WARNING]
> This plugin was designed with SourceMods in mind, meaning mods loaded through the `sourcemods` folder in Steam. This is not for Portal 2 mods on Steam's store, which can be installed through Steam.
>
> While this plugin should be ok for a Steam Portal 2 mod, note that:
>
> 1. I do not know if Valve will allow such modifications on the Steam platform, especially if it involves one of their games.
> 2. Compatibility will not be 100%, so expect bugs or other issues.
> 3. If you are using this plugin with your mod, I am not responsible if anything bad happens to your mod's page on Steam.
>
> Ideally, if you wish to have some of these features but want to be able to have your mod on Steam, wait for P2:CE to release rather than using this. 
> 
> I will, in the future, try to add custom elements involving interactions with Steam's API, like achievements, but for now, I don't expect this to work correctly with Steam mods. Plus, in the end, P2:CE will do this properly in the future.

### Features Implemented/Potentially Plan to Implement:

* Stopped workshop maps from being downloaded by the game because the base game's workshop folder isn't on path for SourceMods. Because of this, it tries to get ***ALL*** of them at once, which causes the game to be unstable and occasionally crash.
* Fixed human NPC movement so they don't jitter when they move. `cl_localnetworkbackdoor` is automatically set to 0 without manual adjustment.
* Made `r_screenoverlay` ConVar be enabled by default and not need cheats so `env_screenoverlay` entities can work correctly without `sv_cheats` being needed.
* Added a ConVar to enable instant respawns for multiplayer. Skips those moments in the third-person death cam before respawn.
* Fixed linked portal doors causing crashes on multiplayer when players enter them.
* Increased the max VScript VM runtime so that VScript operations like `for` and `while` loops run without problems. This helps give more leeway in doing things in VScript without the engine complaining and shutting down the rest of the script, spewing errors in the console. While it depends on how the script was programmed and what it is doing, I occasionally notice this, and this fix helps eliminate it.
* Fixed the player flashlight impulse ConCommand, so it no longer requires cheats to be enabled. All that is needed is a key bound to `impulse 100`. However, the flashlight doesn't work perfectly with portals, snapping to odd positions when moving through and unable to shine through portals.
* **(NOT IMPLEMENTED)** Add a ConVar to disable multiplayer death icons.
* **(NOT IMPLEMENTED)** Allow more than one env_projectedtexture to be on at once. However, the engine limit of eight is still enforced! That can not be fixed.
* **(NOT IMPLEMENTED)** Option to turn on or off puzzle maker functionality. Most mods don't use the puzzle maker, but it still loads in. The plugin will try to disable as much of it as possible so the console is not filled with any puzzle-maker-related errors, messages, or other additional bloat.
* **(NOT IMPLEMENTED)** Custom Discord RPC for Portal 2 SourceMods.
* **(NOT IMPLEMENTED)** Additional achievement support and features. Like custom achievements that span multiple maps and have more parts than Door Prize and Smash TV.
* **(NOT IMPLEMENTED)** As informed by MarvinG, `env_sun` disappears when over player crosshair. I have not been able to confirm this and have never noticed it in general.
* **(NOT IMPLEMENTED)** When in water, the player is pushed to the world's origin.
* **(NOT IMPLEMENTED)** Fix SourceMods only getting a small 2 MB of memory for material system render context allocation, increasing it to 6 MB. This could happen if this gets ported to SAR or P2SRM.
* **(NOT IMPLEMENTED)** Linux support.
* **(NOT IMPLEMENTED)** Implement some VScript functions and hooks to capture specific events, turn ConVars on or off, and fix and add new VScript functions.
* **(NOT IMPLEMENTED)** Make main menu backgrounds switch accordingly to the act number indicated in the map name ("sp_a3").
* **(NOT IMPLEMENTED)** Have a config file to turn specific fixes and features on or off.
* **(NOT IMPLEMENTED)** Reimplement/fix map backgrounds for Portal 2.

### Download and Install Instructions:

Download the latest `addons.zip` under the `Releases` tab on GitHub and extract its folder into the base directory of your SourceMod. Portal 2 should automatically start the plugin up with the game. You should be able to see log messages from the plugin in the developer console if it is loaded. That's it. Please create an Issue post on the plugin's repository if there are any issues. I'll get to it within the next 20 years. (Hopefully not).

> [!IMPORTANT]
> If Portal 2: Multiplayer Mod is running along with P2SM++, P2SM++'s hooks, configs, and settings will take priority over Portal 2: Multiplayer Mod's. If P2SM++ patches the same things that P2:MM does, P2:MM ignores patching as P2SM++ has already done it. Note: P2:MM assumes that it is being loaded second based on how it is loaded normally with the launcher and with a `tempcontent` folder.

**As of writing, 11/10/2024, the plugin is only compatible with Windows. The signature scanner it uses still needs to be compatible with Linux, plus overall, the way the P2:MM plugin is structured (this plugin is built off of some of the P2:MM plugin code as a base), it's impossible to compile for Linux anyway. Literally 1984, I know, but I don't have enough time or experience to figure it out by myself. Linux users should still be able to get away with using Proton or similar to be able to use the plugin's Windows `.dll`. Hopefully, one day, this can be fixed...**

**Update 3/24/2025: I plan to move this over to a forked version of SAR using its code base as a base for this plugin. Its structure is much better, it has more engine functionality accessible in the plugin, and it compiles for Linux. However, do not expect this switch to happen anytime soon. Thank you for being so patient.**

***
Please credit Orsell/OrsellGaming, Nanoman2525, NULLderef, and/or the `Portal 2: Multiplayer Mod Team` if you use this plugin or its code in any way with your SourceMod.
You'll also need to include this repository and P2:MM's license file with the plugin.
Both licenses will be included with the `addon.zip` file you download to get the plugin.
***
