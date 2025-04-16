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
extern void (__fastcall* CPortal_Player__PlayerDeathThink_orig)(CPortal_Player* thisptr);
void __fastcall CPortal_Player__PlayerDeathThink_hook(CPortal_Player* thisptr);

// UTIL_GetLocalPlayer Dedicated Server Fix Hook.
// Hook does follow original naming convention so it can be used like a function.
extern CBasePlayer* (__cdecl* UTIL_GetLocalPlayer_orig)();
CBasePlayer* __cdecl UTIL_GetLocalPlayer();

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