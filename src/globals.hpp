//===========================================================================//
//
// Author: Orsell
// Purpose: Global functions & variables used repeatedly throughout the plugin
//
//===========================================================================//

#pragma once

#include <comdef.h>

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
	for (int (i) = 1; (i) <= Utils::CurPlayerCount(); (i)++)



//---------------------------------------------------------------------------------
// Public variables.
//---------------------------------------------------------------------------------


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