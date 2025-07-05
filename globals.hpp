//===========================================================================//
//
// Author: Orsell
// Purpose: Global functions & variables used repeatedly throughout the plugin
//
//===========================================================================//

#pragma once

#include "eiface.h"
#include "cdll_int.h"
#include "vscript/ivscript.h"
#include "engine/iserverplugin.h"
#include "game/server/iplayerinfo.h"
#include "public/steam/steamclientpublic.h"
#include "irecipientfilter.h"

#include "scanner.hpp"

#include <comdef.h>

// Forward references for class types unable to be accessed directly.
class CBasePlayer;
class CPortal_Player;
class CBaseEntity;
class CWorkshopManager;
class CEnvProjectedTexture;

// Color macros for console printing.
#define P2SMPLUSPLUS_PLUGIN_VERSION "1.1.1" // Update this when a new version of the plugin is released.
#define P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR  Color(100, 192, 252, 255) // Light Blue

#define CUR_MAPFILE_NAME STRING(g_pGlobals->mapname)
#define MAX_PLAYERS g_pGlobals->maxClients

// Used for autocomplete console commands.
#define COMMAND_COMPLETION_MAXITEMS		64
#define COMMAND_COMPLETION_ITEM_LENGTH	64

// A macro to iterate through all ConVars and ConCommand in the game.
// Thanks to Nanoman2525 for this.
#define FOR_ALL_CONSOLE_COMMANDS(pCommandVarName) \
    ConCommandBase* m_pConCommandList = *reinterpret_cast<ConCommandBase**>((uintptr_t)g_pCVar + 0x30); /* CCvar::m_pConCommandList */ \
    for (ConCommandBase* (pCommandVarName) = m_pConCommandList; \
	pCommandVarName; (pCommandVarName) = *reinterpret_cast<ConCommandBase**>(reinterpret_cast<uintptr_t>(pCommandVarName) + 0x04)) /* ConCommandBase::m_pNext (private variable) */

// Macro to iterate through all players on the server.
#define FOR_ALL_PLAYERS(i) \
	for (int (i) = 1; (i) <= CURPLAYERCOUNT(); (i)++)

// Log levels for all log functions.
typedef enum LogLevels : std::uint8_t
{
	INFO = 0,
	WARNING,
	ERRORR // Have to use ERRORR because of macro by the Windows API. Yeah I know, I hate it too.
} LogLevel;


// Player team enum.
enum : std::uint8_t
{
	TEAM_SINGLEPLAYER = 0,
	TEAM_SPECTATOR,
	TEAM_RED,
	TEAM_BLUE
};

// ClientPrint msg_dest macros.
enum : std::uint8_t
{
	HUD_PRINTNOTIFY	= 1, // Works same as HUD_PRINTCONSOLE
	HUD_PRINTCONSOLE,
	HUD_PRINTTALK,
	HUD_PRINTCENTER
};

//---------------------------------------------------------------------------------
// Public variables.
//---------------------------------------------------------------------------------
// Windows API Window Handle
static HWND hWnd;

//---------------------------------------------------------------------------------
// Interfaces from the engine.
//---------------------------------------------------------------------------------
extern IVEngineServer*			engineServer;
extern IVEngineClient*			engineClient;
extern CGlobalVars*				g_pGlobals;
extern IPlayerInfoManager*		g_pPlayerInfoManager;
// extern IScriptVM*			g_pScriptVM;
// extern IServerTools*			g_pServerTools;
// extern IGameEventManager2*	g_pGameEventManager_;
// extern IServerPluginHelpers*	g_pPluginHelpers;
// extern IFileSystem*			g_pFileSystem;

//---------------------------------------------------------------------------------
// UTIL functions.
//---------------------------------------------------------------------------------
int					UserIDToPlayerIndex(int userid);
const char*			GetPlayerName(int playerIndex);
int					GetSteamID(int playerIndex);
int					GetConVarInt(const char* cvName);
const char*			GetConVarString(const char* cvName);
void				SetConVarInt(const char* cvName, int newValue);
void				SetConVarString(const char* cvName, const char* newValue);
bool				IsBot(int playerIndex);
int					GetBotCount();
int					CURPLAYERCOUNT();
HSCRIPT				INDEXHANDLE(int iEdictNum);

// Logging function.
void Log(LogLevel level, bool dev, const char* pMsgFormat, ...);

//---------------------------------------------------------------------------------
// Player recipient filter.
//---------------------------------------------------------------------------------
class CPlayerFilter : public IRecipientFilter
{
public:
	CPlayerFilter(): recipients{} { recipientCount = 0; }
	~CPlayerFilter() override = default;

	bool IsReliable() const override { return false; }
	bool IsInitMessage() const override { return false; }

	int GetRecipientCount() const override { return recipientCount; }

	int GetRecipientIndex(const int slot) const override
	{
		return (slot < 0 || slot >= recipientCount) ? -1 : recipients[slot];
	}

	void AddPlayer(const int playerIndex)
	{
		if (recipientCount < 256)
		{
			recipients[recipientCount] = playerIndex;
			recipientCount++;
		}
	}

private:
	int recipients[256];
	int recipientCount;
};

// If String Equals String helper function. Taken from utils.h.
inline bool FStrEq(const char* sz1, const char* sz2)
{
	return (V_stricmp(sz1, sz2) == 0);
}

// If String Has Substring helper function. Taken from utils.h.
inline bool FSubStr(const char* sz1, const char* search)
{
	return (V_strstr(sz1, search));
}

//---------------------------------------------------------------------------------
// Purpose: Entity edict to entity index. Taken from utils.h.
//---------------------------------------------------------------------------------
inline int EDICTINDEX(const edict_t* pEdict)
{
	if (!pEdict)
		return 0;
	const int edictIndex = pEdict - g_pGlobals->pEdicts;
	Assert(edictIndex < MAX_EDICTS && edictIndex >= 0);
	return edictIndex;
}

//---------------------------------------------------------------------------------
// Purpose: Entity to entity index.
//---------------------------------------------------------------------------------
inline int ENTINDEX(CBaseEntity* pEnt)
{
	static auto ENTINDEX_ = reinterpret_cast<int (__cdecl*)(CBaseEntity*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 8B 45 ?? 85 C0 74 ?? 8B 40 ?? 85 C0 74 ?? 8B 0D"));
	return ENTINDEX_(pEnt);
}

//---------------------------------------------------------------------------------
// Purpose: Entity index to entity edict. Taken from utils.h.
//---------------------------------------------------------------------------------
inline edict_t* INDEXENT(const int iEdictNum)
{
	Assert(iEdictNum >= 0 && iEdictNum < MAX_EDICTS);
	if (g_pGlobals->pEdicts)
	{
		edict_t* pEdict = g_pGlobals->pEdicts + iEdictNum;
		if (pEdict->IsFree())
			return nullptr;
		return pEdict;
	}
	return nullptr;
}

//---------------------------------------------------------------------------------
// Purpose: Returns the current game directory. Ex. portal2
//---------------------------------------------------------------------------------
inline const char* GetGameMainDir()
{
	return CommandLine()->ParmValue("-game", CommandLine()->ParmValue("-defaultgamedir", "portal2"));
}

//---------------------------------------------------------------------------------
// Purpose: Returns the current root game directory. Ex. Portal 2
//---------------------------------------------------------------------------------
inline const char* GetGameRootDir()
{
	static char baseDir[MAX_PATH] = { 0 };
	const std::string fullGameDirectoryPath = engineClient->GetGameDirectory();

	// Find last two backslashes
	const size_t firstSlash = fullGameDirectoryPath.find_last_of('\\');
	const size_t secondSlash = fullGameDirectoryPath.find_last_of('\\', firstSlash - 1);
	const std::string tempBaseDir = fullGameDirectoryPath.substr(secondSlash + 1, firstSlash - secondSlash - 1);
	V_strcpy(baseDir, tempBaseDir.c_str()); // Copy to static buffer

	return baseDir;
}

//---------------------------------------------------------------------------------
// Purpose: Returns true if a game session is running.
//---------------------------------------------------------------------------------
inline bool IsGameActive()
{
	const bool m_activeGame = **Memory::Scanner::Scan<bool**>(ENGINEDLL, "C6 05 ?? ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 0F B6 96", 2);
	return m_activeGame;
}

//---------------------------------------------------------------------------------
// Purpose: Returns true if a game session is shutting down or has been shutdown.
//---------------------------------------------------------------------------------
inline bool IsGameShutdown()
{
	const bool bIsGameShuttingDown = reinterpret_cast<bool(__cdecl*)()>(Memory::Scanner::Scan<void*>(ENGINEDLL, "B8 05 00 00 00 39 05"))();
	return bIsGameShuttingDown;
}
