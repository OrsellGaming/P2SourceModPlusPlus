//===========================================================================//
//
// Author: Orsell
// Purpose: Interfaced functions and hooks from the Portal 2 engine for the plugin to use.
// 
//===========================================================================//

#include "stdafx.hpp"
#include "sdk.hpp"

#include "utils/loggingsystem.hpp"
#include "memory.hpp"
#include "commands.hpp"
#include "p2sm.hpp"

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
	//engineServer-EmitAmbientSound>(playerIndex)
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

// TODO: Replace unstable hook with byte patch.
/**
 * @brief Stop the UGC manager from automatically download workshop maps. Simply return doing nothing so that nothing gets updated and therefore nothing gets downloaded.
 * @warning This makes the game extremely unstable if the plugin is unloaded while the game is running.
 */
void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisPtr);
void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr) { }

}
{
}