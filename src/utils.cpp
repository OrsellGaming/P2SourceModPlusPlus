/*******************************************************************
* @file   utils.cpp
* @brief  Util functions.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "utils.hpp"

#include "modules/cbaseentity.hpp"

#include "globals.hpp"

/**
 * @brief Get the player's entity index by their user ID.
 * @param userid User ID of player.
 * @return Player entity index.
 */
int Utils::UserIDToPlayerIndex(const int userid)
{
    for (int i = 1; i <= MAX_PLAYERS; i++)
    {
        const Edict* pEdict = nullptr;
        if (i >= 0 && i < g_pGlobals->maxEntities)
            pEdict = (g_pGlobals->pEdicts + i);

        if (engineServer->GetPlayerUserId(pEdict) == userid)
            return i;
    }
    return 0; // Return 0 if the index can't be found
}

/**
 * @brief Get player username by their entity index.
 * @param playerIndex Player index.
 * @return Player's username.
 */
const char* Utils::GetPlayerName(const int playerIndex)
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

/**
 * @brief Get the account ID component of player SteamID by the player's entity index.
 * @param playerIndex Player index.
 * @return Steam Account ID of player.
 */
int Utils::GetSteamID(const int playerIndex)
{
	Edict* pEdict = nullptr;
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

	return static_cast<int>(pSteamID->GetAccountID());
}

/**
 * @brief Get the integer value of a ConVar.
 * @param cvName Name of ConVar to get.
 * @return Integer value of ConVar.
 */
int Utils::GetConVarInt(const char* cvName)
{
	const ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, R"(Could not find ConVar: "%s"! Returning ""!)", cvName);
		return -1;
	}

	return pVar->GetInt();
}

/**
 * @brief Get the string value of a ConVar.
 * @param cvName Name of ConVar to get.
 * @return String value of ConVar.
 */
const char* Utils::GetConVarString(const char* cvName)
{
	const ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, R"(Could not find ConVar: "%s"! Returning ""!)", cvName);
		return "";
	}

	return pVar->GetString();
}

/**
 * @brief Set integer value of a ConVar.
 * @param cvName Name of ConVar to set value of.
 * @param newValue New value for ConVar.
 */
void Utils::SetConVarInt(const char* cvName, const int newValue)
{
	ConVar* pVar = g_pCVar->FindVar(cvName);
	if (!pVar)
	{
		Log(WARNING, false, "Could not set ConVar: \"%s\"!", cvName);
		return;
	}
	pVar->SetValue(newValue);
}

/**
 * @brief Set string value of a ConVar.
 * @param cvName Name of ConVar to set value of.
 * @param newValue New value for ConVar.
 */
void Utils::SetConVarString(const char* cvName, const char* newValue)
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

/**
 * @brief Check if player is a bot.
 * @param playerIndex Player index.
 * @return Returns true if player is a bot.
 */
bool Utils::IsBot(const int playerIndex)
{
	player_info_t playerInfo;
	if (!engineServer->GetPlayerInfo(playerIndex, &playerInfo))
	{
		Log(WARNING, true, R"(Couldn't retrieve player info of player index "%i" in IsBot!)", playerIndex);
		return false;
	}

	return playerInfo.fakeplayer;
}

/**
 * @brief Get the number of bots in the server.
 * @return Number of bots in the server.
 */
int Utils::GetBotCount()
{
	int botCount = 0;
	FOR_ALL_PLAYERS(i)
	{
		if (IsBot(i))
			botCount++;
	}
	return botCount;
}

/**
 * @brief Get the current player count on the server.
 * @return Number of players in the server.
 */
int Utils::CurPlayerCount()
{
	int playerCount = 0;
	for (int i = 1; i <= MAX_PLAYERS; i++)
	{
		if (Utils::PlayerByIndex(i))
			playerCount++;
	}
	return playerCount;
}

/**
 * @brief Entity index to VScript script handle.
 * @param entityIndex Entity index.
 * @return VScript script handle.
 */
HSCRIPT Utils::EntIndexScriptHandle(const int entityIndex)
{
	Edict* pEdict = IndexToEdict(entityIndex);
	if (!pEdict->GetUnknown())
		return nullptr;

	CBaseEntity* pBaseEntity = pEdict->GetUnknown()->GetBaseEntity();
	if (!pBaseEntity)
		return nullptr;

	return CBaseEntity::GetScriptInstance(pBaseEntity);
}

/**
 * @brief Get the player's CBasePlayer class by entity index.
 * @param playerEntityIndex Player entity index.
 * @return Pointer to player's CBasePlayer class.
 */
CBasePlayer* Utils::PlayerByIndex(const int playerEntityIndex)
{
	static auto playerByIndex = reinterpret_cast<CBasePlayer* (__cdecl*)(int)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 8B 4D 08 33 C0 85 C9 7E 30"));
	return playerByIndex(playerEntityIndex);
}

/**
 * @brief Send or print various kinds of styles of messages to players. msg_dest are defined macros in globals.hpp.
 * @param player Player's CBasePlayer class.
 * @param msgDest Where message will be sent.
 * @param msg Message to send.
 * @attention The param# parameters are for formatting. Only four max due to hour the message is programmed to be networked.
 * @param param1 Formatting. Defaults to nullptr.
 * @param param2 Formatting. Defaults to nullptr.
 * @param param3 Formatting. Defaults to nullptr.
 * @param param4 Formatting. Defaults to nullptr.
 */
void Utils::ClientPrint(CBasePlayer* player, const MessageDestination msgDest, const char* msg, const char* param1, const char* param2, const char* param3, const char* param4)
{
	static auto clientPrint = reinterpret_cast<void (__cdecl*)(CBasePlayer*, int, const char*, const char*, const char*, const char*, const char*)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 83 EC 20 56 8B 75 08 85 F6 74 4C"));
	clientPrint(player, msgDest, msg, param1, param2, param3, param4);
}

/**
 * @brief Show text on screen just like game_text does.
 * @param pPlayer Pointer to player.
 * @param textParms HudMessageParams struct passed with the settings for the displayed hud message.
 *					If not supplied, defaults to default game_text entity parameters.
 * @param pMessage Message to display.
 */
void Utils::HudMessage(CBasePlayer* pPlayer, const char* pMessage, const HudMessageParams& textParms)
{
	static auto hudMessage = reinterpret_cast<void (__cdecl*)(CBasePlayer*, const HudMessageParams&, const char*)>(Memory::Scan(MODULE_SERVER, "55 8B EC 83 EC 20 8D 4D ? E8 ? ? ? ? 8B 45 ? 8D 4D ? 85 C0 74 ? 50 E8 ? ? ? ? EB ? E8 ? ? ? ? 56"));
	hudMessage(pPlayer, textParms, pMessage);
}

/**
 * @brief Get the CBasePlayer of the player who executed the ConVar or ConCommand.
 *		  Must be executed inside a ConCommand/ConVar function context.
 * @return Pointer to player's CBasePlayer class.
 */
CBasePlayer* Utils::GetCommandClient()
{
	static auto getCommandClient = reinterpret_cast<CBasePlayer* (__cdecl*)()>(Memory::Scan(MODULE_SERVER, "A1 ? ? ? ? 40 85 C0"));
	return getCommandClient();
}

/**
 * @brief Get the client index of the player who executed the ConVar or ConCommand.
 * @warning This is not the player's entity index! Entity index is +1 bigger.
 * @return Player's client index.
 */
int Utils::GetCommandClientIndex()
{
	static auto getCommandClientIndex = reinterpret_cast<int (__cdecl*)()>(Memory::Scan(MODULE_SERVER, "A1 ? ? ? ? 40 C3"));
	return getCommandClientIndex();
}

/**
 * @brief Change a entities origin in the world.
 * @param entity Entity to set the new origin of.
 * @param vecOrigin New origin of entity.
 * @param fireTriggers Have the moved entity trigger triggers.
 */
void Utils::SetOrigin(CBaseEntity* entity, const Vector& vecOrigin, const bool fireTriggers)
{
	static auto setOrigin = reinterpret_cast<void (__cdecl*)(CBaseEntity*, const Vector&, bool)>(Memory::Scan(MODULE_SERVER, "55 8B EC 8B 45 0C 56 8B 75 08 50 8B"));
	setOrigin(entity, vecOrigin, fireTriggers);
}

/**
 * @brief CBaseEntity to entity index.
 * @param entity Pointer to entity.
 * @return Entity index of entity.
 */
int Utils::EntityIndex(CBaseEntity* entity)
{
	static auto entIndex = reinterpret_cast<int (__cdecl*)(CBaseEntity*)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 8B 45 ? 85 C0 74 ? 8B 40 ? 85 C0 74 ? 8B 0D"));
	return entIndex(entity);
}

/**
 * @brief Entity edict to entity index.
 * @param pEdict Pointer to edict.
 * @return Entity index for edict.
 */
int Utils::EdictIndex(const Edict* pEdict)
{
	if (!pEdict)
		return 0;
	const int edictIndex = pEdict - g_pGlobals->pEdicts;
	assert(edictIndex < MAX_EDICTS && edictIndex >= 0);
	return edictIndex;
}

/**
 * @brief Entity index to entity edict.
 * @param entityIndex Entity index.
 * @return Edict of entity index.
 */
Edict* Utils::IndexToEdict(const int entityIndex)
{
	assert(entityIndex >= 0 && entityIndex < MAX_EDICTS);
	if (g_pGlobals->pEdicts)
	{
		Edict* pEdict = g_pGlobals->pEdicts + entityIndex;
		if (pEdict->IsFree())
			return nullptr;
		return pEdict;
	}
	return nullptr;
}

/**
 * @brief Convert CBaseEntity to entity edict.
 * @param entity Pointer to entity.
 * @return Edict of CBaseEntity.
 */
Edict* Utils::EntityToEdict(CBaseEntity* entity)
{
	return IndexToEdict(EntityIndex(entity));
}

/**
 * @brief Returns the current game directory. Ex. "portal2"
 * @return The current game directory.
 */
const char* Utils::GetGameMainDir()
{
	return CommandLine()->ParmValue("-game", CommandLine()->ParmValue("-defaultgamedir", "portal2"));
}

/**
 * @brief Return the current root game directory. Ex. "Portal 2"
 * @return The current root game directory.
 */
const char* Utils::GetGameRootDir()
{
	static char baseDir[MAX_PATH] = { 0 };
	const std::string fullGameDirectoryPath = engineClient->GetGameDirectory();

	// Find last two backslashes
	const size_t firstSlash = fullGameDirectoryPath.find_last_of('\\');
	const size_t secondSlash = fullGameDirectoryPath.find_last_of('\\', firstSlash - 1);
	const std::string tempBaseDir = fullGameDirectoryPath.substr(secondSlash + 1, firstSlash - secondSlash - 1);
	std::strcpy(baseDir, tempBaseDir.c_str());

	return baseDir;
}

/**
 * @brief Check if a game session is running.
 * @return Returns true if a game session is running.
 */
bool Utils::IsGameActive()
{
	const bool m_activeGame = **Memory::Scan<bool**>(MODULE_ENGINE, "C6 05 ? ? ? ? ? C6 05 ? ? ? ? ? 0F B6 96", 2);
	return m_activeGame;
}

/**
 * @brief Check if the game session is shutdown or is in the process of shutting down.
 * @return Returns true if a game session is shutting down or has been shutdown.
 */
bool Utils::IsGameShutdown()
{
	const bool bIsGameShuttingDown = reinterpret_cast<bool(__cdecl*)()>(Memory::Scan<void*>(MODULE_ENGINE, "B8 05 00 00 00 39 05"))();
	return bIsGameShuttingDown;
}

