/*******************************************************************
* @file   globals.cpp
* @brief  Global functions & variables used repeatedly throughout the plugin.
* @author Orsell
* @date   03 2025
*********************************************************************/

#include "stdafx.hpp"
#include "globals.hpp"

//---------------------------------------------------------------------------------
// Interfaces from the engine.
//---------------------------------------------------------------------------------
IVEngineServer*			engineServer = nullptr;
IVEngineClient*			engineClient = nullptr;
CGlobalVars*			g_pGlobals   = nullptr;
IPlayerInfoManager*		g_pPlayerInfoManager = nullptr;
// IScriptVM*				g_pScriptVM;
//IServerTools*			g_pServerTools = nullptr;
// IGameEventManager2*		g_pGameEventManager_;
//IServerPluginHelpers*	g_pPluginHelpers = nullptr;
// IFileSystem*				g_pFileSystem;
