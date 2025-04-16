//===========================================================================//
//
// Author: Orsell
// Purpose: Interfaced functions and hooks from the Portal 2 engine for the plugin to use.
// 
//===========================================================================//

#pragma once

#include "globals.hpp"

// UTIL_HudMessage message parameters struct. Based on the one from util.h.
// See Valve Developer Community for game_text to see which field does what:
// https://developer.valvesoftware.com/wiki/Game_text
typedef struct
{
	float		x, y;
	int			effect;
	byte		r1, g1, b1, a1;
	byte		r2, g2, b2, a2;
	float		fadeinTime, fadeoutTime, holdTime;
	float		fxTime;
	int			channel;
} HudMessageParams;

//---------------------------------------------------------------------------------
// Hooked game functions.
//---------------------------------------------------------------------------------

// Respawn Hooks.
extern void (__fastcall* CPortal_Player__PlayerDeathThink_orig)(CPortal_Player* thisPtr);
void __fastcall CPortal_Player__PlayerDeathThink_hook(CPortal_Player* thisPtr);

// Flashlight Hooks.
extern bool (__fastcall* CPortal_Player__FlashlightTurnOn_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
bool __fastcall CPortal_Player__FlashlightTurnOn_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);
extern void (__fastcall* CPortal_Player__FlashlightTurnOff_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
void  __fastcall CPortal_Player__FlashlightTurnOff_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);

// Workshop download stopping hooks.
extern bool (__fastcall* CWorkshopManager__CreateFileDownloadRequest_orig)(CWorkshopManager* thisPtr, void* edx,
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
	bool bForceUpdate);

// env_projectedtexture enforcement function hook.
// extern void (__fastcall* CEnvProjectedTexture__EnforceSingleProjectionRules_orig)(CEnvProjectedTexture* thisPtr, void* edx, bool bWarnOnEnforcement);
// void  __fastcall CEnvProjectedTexture__EnforceSingleProjectionRules_hook(CEnvProjectedTexture* thisPtr, void* edx, bool bWarnOnEnforcement);

//---------------------------------------------------------------------------------
// Interfaced game functions.
//---------------------------------------------------------------------------------
// UTIL functions
CBasePlayer* UTIL_PlayerByIndex(int playerIndex);
void UTIL_ClientPrint(CBasePlayer* player, int msg_dest, const char* msg_name, const char* param1 = nullptr, const char* param2 = nullptr, const char* param3 = nullptr, const char* param4 = nullptr);
void UTIL_HudMessage(CBasePlayer* pPlayer, const HudMessageParams& textparms, const char* pMessage);
CBasePlayer* UTIL_GetCommandClient();
int UTIL_GetCommandClientIndex();

// CBaseEntity functions
void CBaseEntity__RemoveEntity(CBaseEntity* pEntity);
int CBaseEntity__GetTeamNumber(CBasePlayer* pPlayer);
HSCRIPT CBaseEntity__GetScriptScope(CBaseEntity* entity);
HSCRIPT CBaseEntity__GetScriptInstance(CBaseEntity* entity);

// CBasePlayer functions
void CBasePlayer__ShowViewPortPanel(int playerIndex, const char* name, bool bShow = true, KeyValues* data = nullptr);

// CPortal_Player functions
void CPortal_Player__RespawnPlayer(int playerIndex);
void CPortal_Player__SetFlashlightState(int playerIndex, bool enable);