# ***P2SourceModPlusPlus*** (Formerly ***WorkshopStopper9000***)

## ***A Source Engine Plugin for Portal 2 Source Mods that fixes and adds some QoL features for them!***

> I originally planned `WorkshopStopper9000` to simply fix the workshop downloading issue, but I hope to add a bit more features and fixes so now it has become `P2SourceModPlusPlus`. Hopefully I can implement some good features that will be used by the community until the fated day Portal 2: Community Edition is released. Basically this is gonna be a temporarily solution for mod makers.

### Features Implemented/Plan to Implement:

* Stopped workshop maps from being downloaded by the game because the workshop folder isn't on path for Source Mods. Because of this it tries to get ***ALL*** of them at once, which caused the game to be unstable and on occasion crash.
* **(NOT IMPLEMENTED)** Fix human NPC movement so they don't jitter when they move.
* **(NOT IMPLEMENTED)** Option to turn on or off Puzzlemaker functionality. Most mods don't use the puzzlemaker, but it still loads in. The plugin will try to disable as much of it as possible so console is not filled with any puzzlemaker related errors or messages or any other additional bloat.
* **(NOT IMPLEMENTED)** Custom Discord RPC for Portal 2 Source Mods.
* **(NOT IMPLEMENTED)** Additional achievement support and features. Like custom achievements that span multiple maps that have more parts than Door Prize and Smash TV.

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
