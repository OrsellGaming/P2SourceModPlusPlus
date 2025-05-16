//===========================================================================//
//
// Author: Orsell
// Purpose: Interfaced functions and hooks from the Portal 2 engine for the plugin to use.
// 
//===========================================================================//

#include "sdk.hpp"

#include "scanner.hpp"
#include "p2sm.hpp"

static ConVar p2sm_instantrespawn("p2sm_instantrespawn", "0", FCVAR_NONE, "Whether respawning should be instant or not in multiplayer.");

//---------------------------------------------------------------------------------
// Purpose: For hooking onto the function that is called before a player respawns to skip the delay
//			that is usual there and instead force a instant respawn of the player.
//---------------------------------------------------------------------------------
void (__fastcall* CPortal_Player__PlayerDeathThink_orig)(CPortal_Player* thisPtr);
void __fastcall CPortal_Player__PlayerDeathThink_hook(CPortal_Player* thisPtr)
{
	if (p2sm_instantrespawn.GetBool())
	{
		CPortal_Player__RespawnPlayer(ENTINDEX(reinterpret_cast<CBaseEntity*>(thisPtr)));
		return;
	}
	CPortal_Player__PlayerDeathThink_orig(thisPtr);
}

//---------------------------------------------------------------------------------
// Purpose: Change out the original Flashlight turn and off with our versions.
//			TODO: Have flashlight on and off sound work again.
//---------------------------------------------------------------------------------
bool (__fastcall* CPortal_Player__FlashlightTurnOn_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
bool __fastcall CPortal_Player__FlashlightTurnOn_hook(CPortal_Player* thisPtr,  void* edx, bool playSound)
{
	const int playerIndex = ENTINDEX(reinterpret_cast<CBaseEntity*>(thisPtr));
	if (playerIndex <= 0 || playerIndex > MAX_PLAYERS)
	{
		Log(WARNING, false, "CPortal_Player::FlashlightTurnOn was called with a invalid player!");
		return false;
	}
	
	CPortal_Player__SetFlashlightState(playerIndex, true);
	// engineServer->EmitAmbientSound(playerIndex)
	return true;
}

void (__fastcall* CPortal_Player__FlashlightTurnOff_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
void  __fastcall CPortal_Player__FlashlightTurnOff_hook(CPortal_Player* thisPtr,  void* edx, bool playSound)
{
	const int playerIndex = ENTINDEX(reinterpret_cast<CBaseEntity*>(thisPtr));
	if (playerIndex <= 0 || playerIndex > MAX_PLAYERS)
	{
		Log(WARNING, false, "CPortal_Player::FlashlightTurnOff was called with a invalid player!");
		return;
	}
	
	CPortal_Player__SetFlashlightState(playerIndex, false);
	// engineServer->EmitAmbientSound(playerIndex)
	return;
}

//---------------------------------------------------------------------------------
// Purpose: Stop the UGC manager from automatically download workshop maps.
//			Simply do nothing so that nothing gets updated and therefore nothing gets downloaded.
//			!WARNING! This makes the game extremely unstable if the plugin is unloaded while the game is running.
//---------------------------------------------------------------------------------
bool (__fastcall* CWorkshopManager__CreateFileDownloadRequest_orig)(CWorkshopManager* thisPtr, void* edx,
	uint64 hFileHandle, 
	uint64 fileID,
	const char *lpszDirectory, 
	const char *lpszFilename, 
	uint32 unPriority, 
	uint32 unTimeLastUpdated, 
	bool bForceUpdate);
bool  __fastcall CWorkshopManager__CreateFileDownloadRequest_hook(CWorkshopManager* thisPtr, void* edx,
	uint64 hFileHandle, 
	uint64 fileID,
	const char *lpszDirectory, 
	const char *lpszFilename, 
	uint32 unPriority, 
	uint32 unTimeLastUpdated, 
	bool bForceUpdate)
{
	return false;
}

//---------------------------------------------------------------------------------
// Purpose: Stop engine from disabling any other env_projectedtexture entities when one turns on.
//			! Engine limit still exists though with a max of eight env_projectedtextures.
//---------------------------------------------------------------------------------
// void (__fastcall* CEnvProjectedTexture__EnforceSingleProjectionRules_orig)(CEnvProjectedTexture* thisPtr, void* edx, bool bWarnOnEnforcement);
// void  __fastcall  CEnvProjectedTexture__EnforceSingleProjectionRules_hook(CEnvProjectedTexture* thisPtr, void* edx, bool bWarnOnEnforcement) { }

///			 Interfaced UTIL Functions			\\\

//---------------------------------------------------------------------------------
// Purpose: Get the player's base class with its entity index. Thanks to Nanoman2525 for this.
//---------------------------------------------------------------------------------
CBasePlayer* UTIL_PlayerByIndex(const int playerIndex)
{
#ifdef _WIN32
	static auto PlayerByIndex_ = reinterpret_cast<CBasePlayer* (__cdecl*)(int)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 8B 4D 08 33 C0 85 C9 7E 30"));
	return PlayerByIndex_(playerIndex);
#else // Linux support TODO
	return nullptr;
#endif
}

//---------------------------------------------------------------------------------
// Purpose: Show on screen message to players. msg_dest are definSed macros in globals.hpp.
//---------------------------------------------------------------------------------
void UTIL_ClientPrint(CBasePlayer* player, const int msg_dest, const char* msg_name, const char* param1, const char* param2, const char* param3, const char* param4)
{
	static auto ClientPrint_ = reinterpret_cast<void (__cdecl*)(CBasePlayer*, int, const char*, const char*, const char*, const char*, const char*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 83 EC 20 56 8B 75 08 85 F6 74 4C"));
	ClientPrint_(player, msg_dest, msg_name, param1, param2, param3, param4);
}

//---------------------------------------------------------------------------------
// Purpose: Show on text on screen just like game_text does.
//---------------------------------------------------------------------------------
void UTIL_HudMessage(CBasePlayer* pPlayer, const HudMessageParams& textparms, const char* pMessage)
{
	static auto HudMessage_ = reinterpret_cast<void (__cdecl*)(CBasePlayer*, const HudMessageParams&, const char*)>(Memory::Scanner::Scan(SERVERDLL, "55 8B EC 83 EC 20 8D 4D ?? E8 ?? ?? ?? ?? 8B 45 ?? 8D 4D ?? 85 C0 74 ?? 50 E8 ?? ?? ?? ?? EB ?? E8 ?? ?? ?? ?? 56"));
	HudMessage_(pPlayer, textparms, pMessage);
}

//---------------------------------------------------------------------------------
// Purpose: Get the CBasePlayer of the player who executed the ConVar or ConCommand.
//---------------------------------------------------------------------------------
CBasePlayer* UTIL_GetCommandClient()
{
	static auto GetCommandClient_ = reinterpret_cast<CBasePlayer* (__cdecl*)()>(Memory::Scanner::Scan(SERVERDLL, "A1 ?? ?? ?? ?? 40 85 C0"));
	return GetCommandClient_();
}

//---------------------------------------------------------------------------------
// Purpose: Get the client index of the player who executed the ConVar or ConCommand. This is not the player's entity index which is 1 more.
//---------------------------------------------------------------------------------
int UTIL_GetCommandClientIndex()
{
	static auto GetCommandClientIndex_ = reinterpret_cast<int (__cdecl*)()>(Memory::Scanner::Scan(SERVERDLL, "A1 ?? ?? ?? ?? 40 C3"));
	return GetCommandClientIndex_();
}

///			 CBaseEntity Class Functions				\\\

//---------------------------------------------------------------------------------
// Purpose: Self-explanatory. Thanks to Nanoman2525 for this.
//---------------------------------------------------------------------------------
void CBaseEntity__RemoveEntity(CBaseEntity* pEntity)
{
	static auto RemoveEntity_ = reinterpret_cast<void (__cdecl*)(void*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 57 8B 7D 08 85 FF 74 72"));
	RemoveEntity_((reinterpret_cast<IServerEntity*>(pEntity)->GetNetworkable()));
}

//---------------------------------------------------------------------------------
// Purpose: Get team number for the supplied CBasePlayer.
//---------------------------------------------------------------------------------
int CBaseEntity__GetTeamNumber(CBasePlayer* pPlayer)
{
	static auto GetTeamNumber_ = reinterpret_cast<int(__thiscall*)(CBaseEntity*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "8B 81 F4 02 00 00 C3"));
	return GetTeamNumber_(reinterpret_cast<CBaseEntity*>(pPlayer));
}

//---------------------------------------------------------------------------------
// Purpose: Get the script scope of a entity. Thanks to Nullderef/Vista for this.
//---------------------------------------------------------------------------------
HSCRIPT CBaseEntity__GetScriptScope(CBaseEntity* entity)
{
	if (!entity)
		return nullptr;

	return *reinterpret_cast<HSCRIPT*>(reinterpret_cast<uintptr_t>(entity) + 0x33c);
}

//---------------------------------------------------------------------------------
// Purpose: Get the script instance of a entity. Thanks to Nullderef/Vista for this.
//---------------------------------------------------------------------------------
HSCRIPT CBaseEntity__GetScriptInstance(CBaseEntity* entity)
{
	static auto GetScriptInstance_ = reinterpret_cast<HSCRIPT(__thiscall*)(CBaseEntity*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 51 56 8B F1 83 BE 50"));
	if (!GetScriptInstance_)
	{
		Log(WARNING, false, "Could not get script instance for entity!");
		return nullptr;
	}

	return GetScriptInstance_(entity);
}


///			 CBasePlayer Class Functions				\\\

void CBasePlayer__ShowViewPortPanel(const int playerIndex, const char* name, const bool bShow, KeyValues* data)
{
	CBasePlayer* pPlayer = UTIL_PlayerByIndex(playerIndex);
	if (!pPlayer)
	{
		Log(WARNING, false, "Couldn't get player to display view port panel to! playerIndex: %i", playerIndex);
		return;
	}
	static auto ShowViewPortPanel_ = reinterpret_cast<void(__thiscall*)(CBasePlayer*, const char*, bool, KeyValues*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 83 EC 20 53 56 8B F1 57 8D 4D ?? E8 ?? ?? ?? ?? 56"));
	ShowViewPortPanel_(pPlayer, name, bShow, data);
}


///			 CPortal_Player Class Functions				\\\

//---------------------------------------------------------------------------------
// Purpose: Respawn the a player by their entity index.
//---------------------------------------------------------------------------------
void CPortal_Player__RespawnPlayer(const int playerIndex)
{
	CBasePlayer* pPlayer = UTIL_PlayerByIndex(playerIndex);
	if (!pPlayer)
	{
		Log(WARNING, false, "Couldn't get player to respawn! playerIndex: %i", playerIndex);
		return;
	}

	static auto RespawnPlayer_ = reinterpret_cast<void(__thiscall*)(CPortal_Player*)>(Memory::Scanner::Scan<void*>(SERVERDLL, "0F 57 C0 56 8B F1 57 8D 8E"));
	RespawnPlayer_(reinterpret_cast<CPortal_Player*>(pPlayer));
}

//---------------------------------------------------------------------------------
// Purpose: Set the flashlight for a player on or off. Thanks to Nanoman2525 for this.
//			Not a function in the CPortal_Player class, just more grouping it with the
//			class. This does the same thing as the FlashlightTurnOn and FlashlightTurnOff
//			functions in CPortal_Player but done in one function.
//---------------------------------------------------------------------------------
void CPortal_Player__SetFlashlightState(const int playerIndex, const bool enable)
{
	CBasePlayer* pPlayer = UTIL_PlayerByIndex(playerIndex);
	if (!pPlayer)
	{
		Log(WARNING, true, "Couldn't get player to set flashlight state! playerIndex: %i enable: %i", playerIndex, !!enable);
		return;
	}

	if (enable)
		reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 53 8B D9 8B 83 A8"))(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
	else
		reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scanner::Scan<void*>(SERVERDLL, "55 8B EC 53 56 8B 75 08 8B D9 8B 83"))(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
}