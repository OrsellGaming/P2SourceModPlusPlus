//===========================================================================//
//
// Author: Orsell
// Purpose: Global functions & variables used repeatedly throughout the plugin
// 
//===========================================================================//
#include "globals.hpp"

#include "sdk.hpp"
#include "p2sm.hpp"

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

static ConVar p2sm_developer("p2sm_developer", "0", FCVAR_NONE, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// Purpose: Logging for the plugin by adding a prefix and line break.
// Max character limit of 1024 characters.	
// level:	0 = Msg/DevMsg, 1 = Warning/DevWarning, 2 = Error WILL STOP ENGINE!
//---------------------------------------------------------------------------------
void Log(const LogLevel level, const bool dev, const char* pMsgFormat, ...)
{
	if (dev && !p2sm_developer.GetBool() && level != ERRORR) return; // Stop developer messages when p2mm_developer isn't enabled.

	// Take our log message and format any arguments it has into the message.
	va_list argPtr;
	char szFormattedText[1024] = { 0 };
	va_start(argPtr, pMsgFormat);
	V_vsnprintf(szFormattedText, sizeof(szFormattedText), pMsgFormat, argPtr);
	va_end(argPtr);

	// Add a header to the log message.
	char completeMsg[1024] = { 0 };
	V_snprintf(completeMsg, sizeof(completeMsg), "(P2SourceMod++ PLUGIN): %s\n", szFormattedText);

	switch (level)
	{
	case (INFO):
		ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
		return;
	case (WARNING):
		Warning(completeMsg);
		return;
	case (ERRORR):
		Warning("(P2SourceMod++ PLUGIN):\n!!!ERROR ERROR ERROR!!!:\nA FATAL ERROR OCCURED WITH THE ENGINE:\n%s", completeMsg);
		Error(completeMsg);
		return;
	default:
		Warning("(P2SourceMod++ PLUGIN): Log level set outside of INFO-ERRORR, \"%i\". Defaulting to level INFO.\n", level);
		ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
	}
}

//---------------------------------------------------------------------------------
// Purpose: Get the player's entity index by their userid.
//---------------------------------------------------------------------------------
int UserIDToPlayerIndex(const int userid)
{
	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		const edict_t* pEdict = nullptr;
		if (i >= 0 && i < g_pGlobals->maxEntities)
			pEdict = (g_pGlobals->pEdicts + i);

		if (engineServer->GetPlayerUserId(pEdict) == userid)
			return i;
	}
	return 0; // Return 0 if the index can't be found
}

//---------------------------------------------------------------------------------
// Purpose: Get player username by their entity index.
//---------------------------------------------------------------------------------
const char* GetPlayerName(const int playerIndex)
{
	if (playerIndex <= 0 || playerIndex > MAX_PLAYERS)
	{
		Log(WARNING, true, "Invalid index passed to GetPlayerName: %i! Returning ""!", playerIndex);
		return "";
	}

	player_info_t playerInfo;
	if (!engineServer->GetPlayerInfo(playerIndex, &playerInfo))
	{
		Log(WARNING, true, R"(Couldn't retrieve playerInfo of player index in GetPlayerName: %i! Returning ""!)", playerIndex);
		return "";
	}

	return playerInfo.name;
}

//---------------------------------------------------------------------------------
// Purpose: Get the account ID component of player SteamID by the player's entity index.
//---------------------------------------------------------------------------------
int GetSteamID(const int playerIndex)
{
	edict_t* pEdict = nullptr;
	if (playerIndex >= 0 && playerIndex < MAX_PLAYERS)
		pEdict = (g_pGlobals->pEdicts + playerIndex);

	if (!pEdict)
		return -1;

	player_info_t playerInfo;
	if (!engineServer->GetPlayerInfo(playerIndex, &playerInfo))
		return -1;

	const CSteamID* pSteamID = engineServer->GetClientSteamID(pEdict);
	if (!pSteamID || pSteamID->GetAccountID() == 0)
		return -1;

	return pSteamID->GetAccountID();
}

//---------------------------------------------------------------------------------
// Purpose: Self-explanatory.
//---------------------------------------------------------------------------------
int GetConVarInt(const char* cvName)
{
	const ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, R"(Could not find ConVar: "%s"! Returning ""!)", cvName);
		return -1;
	}

	return pVar->GetInt();
}

//---------------------------------------------------------------------------------
// Purpose: Self-explanatory.
//---------------------------------------------------------------------------------
const char* GetConVarString(const char* cvName)
{
	const ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, R"(Could not find ConVar: "%s"! Returning ""!)", cvName);
		return "";
	}

	return pVar->GetString();
}

//---------------------------------------------------------------------------------
// Purpose: Self-explanatory.
//---------------------------------------------------------------------------------
void SetConVarInt(const char* cvName, const int newValue)
{
	ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, "Could not set ConVar: \"%s\"!", cvName);
		return;
	}
	pVar->SetValue(newValue);
}

//---------------------------------------------------------------------------------
// Purpose: Self-explanatory.
//---------------------------------------------------------------------------------
void SetConVarString(const char* cvName, const char* newValue)
{
	ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, R"(Could not set ConVar: "%s"!)", cvName);
		return;
	}
	pVar->SetValue(newValue);
	return;
}

//---------------------------------------------------------------------------------
// Purpose: Returns true if player is a bot.
//---------------------------------------------------------------------------------
bool IsBot(const int playerIndex)
{
	player_info_t playerInfo;
	if (!engineServer->GetPlayerInfo(playerIndex, &playerInfo))
	{
		Log(WARNING, true, R"(Couldn't retrieve player info of player index "%i" in IsBot!)", playerIndex);
		return false;
	}

	return playerInfo.fakeplayer;
}

//---------------------------------------------------------------------------------
// Purpose: Get the number of bots in the server.
//---------------------------------------------------------------------------------
int GetBotCount()
{
	int b = 0;
	FOR_ALL_PLAYERS(i)
	{
		if (IsBot(i))
			b++;
	}
	return b;
}

//---------------------------------------------------------------------------------
// Purpose: Get the current player count on the server.
//---------------------------------------------------------------------------------
int CURPLAYERCOUNT()
{
	int playerCount = 0;
	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		if (UTIL_PlayerByIndex(i))
			playerCount++;
	}
	return playerCount;
}

//---------------------------------------------------------------------------------
// Purpose: Entity index to script handle.
//---------------------------------------------------------------------------------
HSCRIPT INDEXHANDLE(const int iEdictNum)
{
	edict_t* pEdict = INDEXENT(iEdictNum);
	if (!pEdict->GetUnknown())
		return nullptr;
		
	CBaseEntity* pBaseEntity = pEdict->GetUnknown()->GetBaseEntity();
	if (!pBaseEntity)
		return nullptr;
	
	return CBaseEntity__GetScriptInstance(pBaseEntity);
}