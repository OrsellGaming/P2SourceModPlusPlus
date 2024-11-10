# ***WorkshopStopper9000***

## ***A Source Engine Plugin for Portal 2 Source Mods to stop workshop maps from downloading.***

One issue plaguing Source Mods for Portal 2 is the issue where the game sees that the workshop folder is empty, so it re-downloads ***ALL*** of the user's subscribed workshop maps into the Source Mods maps folder. There has been no proper way to stop this from happening, so here is a plugin to solve the issue!

Stick this plugin and its respective `.vdc` file into an `addons` folder in the Source Mod's game directory, and Portal 2 should automatically start the plugin up with the game and thus stop the game from downloading workshop maps. That's it. This took about 20 minutes to make, so please create an Issue post if there are any issues. I'll get to it within the next 20 years.

**As of writing, 11/09/2024, the plugin is only compatible with Windows. The signature scanner it uses still needs to be compatible with Linux, plus overall the way the P2:MM plugin is structured (this plugin is built off of the P2:MM plugin as a base), it's not possible to compile for Linux anyway. Literally 1984, I know, but I don't have enough time or experience to figure it out by myself. Linux users should still be able to get away with using Proton or similar to be able to use the Windows `.dll`. Hopefully one day this can be fixed...**

```
Please give credit to Orsell/OrsellGaming, Nanoman2525, and NULLderef,
as well as the Portal 2: Multiplayer Mod Team if you use this plugin or its code in any way with your Source Mod.
You must also include this repository and the P2:MM's License (see below).
```

```
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
