/*******************************************************************
* @file   patch-hook.cpp
* @brief  Perform hooks and patches for the plugin.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "patch-hook.hpp"

#include "misc.hpp"
#include "utils.hpp"
#include "signatures.hpp"
#include "hook.hpp"

/**
 * @brief Enable and execute the patches for the plugin.
 * @return True if patches executed successfully.
 */
bool Patches::EnablePatches()
{
	try
	{
		// When a player, both client or host, goes through a linked_portal_door entity in multiplayer, the host will crash. This fixes that.
		Log(INFO, true, "Fixing linked portal doors for multiplayer...");
		Memory::ReplacePattern("server", Signature::CPortal_Player_NotifySystemEvent_EventTeleportUserID, Signature::CPortal_Player_NotifySystemEvent_EventTeleportUserID_Patch);
    		
    	// Increase runtime max from 0.03 to 0.05.
    	// Helps add some more leeway to some things we do in VScript without the engine complaining and shutting down the rest of the script.
    	Log(INFO, true, "Patching max runtime for VScript...");
    	Memory::ReplacePattern("vscript", Signature::SQVM_RuntimeTimeout, Signature::SQVM_RuntimeTimeout_Patch);
	
    	// Disable the game auto-disabling others when there is more than one active. Thanks to \n and BetweenReality with help with these.
    	Log(INFO, true, "Patching max amount of projected textures at once and auto disabling of projected textures...");
    	// CEnvProjectedTexture::EnforceSingleProjectionRules
    	Memory::ReplacePattern("server", Signature::CEnvProjectedTexture_EnforceSingleProjectionRules_Loop, Signature::CEnvProjectedTexture_EnforceSingleProjectionRules_Loop_Patch);
    		
    	// CClientShadowMgr::CalculateRenderTargetsAndSizes | Increase the projected texture limit from 1 -> 8
    	Memory::ReplacePattern("client", Signature::CClientShadowMgr_CalculateRenderTargetsAndSizes_PtexLimit, Signature::CClientShadowMgr_CalculateRenderTargetsAndSizes_PtexLimit_Patch);
    	Log(INFO, true, "Increasing shadow map resolution for projected textures...");
		// Increase the resolution shadow maps without tools mode.
    	Memory::ReplacePattern("client", Signature::CClientShadowMgr_CalculateRenderTargetsAndSizes_ShadowRes, Signature::CClientShadowMgr_CalculateRenderTargetsAndSizes_ShadowRes_Patch); 
	
    	// Log(INFO, true, "Patching all projected textures to have volumetrics...");
    	// CShadowMgr::DrawVolumetrics
    	// TODO: Make volumetric intensity customizable.
    	// Memory::ReplacePattern("engine", "0F 85 47 0C 00 00 80 BF 10 02 00 00 00 0F 84 3A 0C 00 00", "0F 84 47 0C 00 00 80 BF 10 02 00 00 00 0F 85 3A 0C 00 00"); // Make volumetrics work for projected textures.
	}
	catch (const std::exception& ex)
	{
		Log(INFO, false, "Failed to perform patch operations! Exception: \"%s\"", ex.what());
		assert(false && "Patch operations failure!");
		return false;
	}

	return true;
}

bool Patches::DisablePatches()
{
	try
	{
		Log(INFO, true, "Un-patching linked portal doors...");
		Memory::ReplacePattern("server", "EB 14 87 04 05 00 00 8B 16", "0F B6 87 04 05 00 00 8B 16");

		Log(INFO, true, "Un-patching max runtime for VScript...");
		Memory::ReplacePattern("vscript", "00 00 00 00 00 00 E0 3F", "00 00 00 E0 51 B8 9E 3F");

		Log(INFO, true, "un-patching max amount of projected textures at once and auto disabling of projected textures...");
		Memory::ReplacePattern("server", "E9 9D 00 00 00 84 95 00 00 00", "8B F0 3B F3 0F 84 95 00 00 00");
		Memory::ReplacePattern("client", "B8 08 00 00 00 90 90 90 5F", "F7 D8 1B C0 83 E0 07 40 5F");

		Log(INFO, true, "Increasing shadow map resolution for projected textures...");
		Memory::ReplacePattern("client", "0F 94 C3 89 8E", "0F 95 C3 89 8E");
	}
	catch (const std::exception& ex)
	{
		Log(INFO, false, "Failed to perform un-patch operations! Exception: \"%s\"", ex.what());
		assert(false && "Un-patch operations failure!");
		return false;
	}

	return true;
}

bool Hooks::EnableHooks()
{
	MH_Initialize();
	
	CUGCFileRequestManager::h_Update->SetFunc<void*>(
		Memory::Scan<void*>(MODULE_CLIENT, "55 8B EC 81 EC 48 01 00 00 57"));
	
	for (const auto hook : Hook::GetHooks())
	{
		Log(INFO, false, "HOOKED: %s", hook->GetName().c_str());
		hook->Enable();
	}
	
	// Hook the death think function so players can be spawned immediate when the p2sm_multiplayer_instantrespawn ConVar is on.
	// Log(INFO, true, "Hooking CPortal_Player::PlayerDeathThink...");
	// MH_CreateHook(
	// 	Memory::Scan<void*>(MODULE_SERVER, "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B ? 89 6C 24 ? 8B EC A1 ? ? ? ? F3 0F 10 40 ? F3 0F 58 05 ? ? ? ? 83 EC 28 56 57 6A 00 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 6A 03"),
	// 	&CPortal_Player__PlayerDeathThink_hook, reinterpret_cast<void**>(&CPortal_Player__PlayerDeathThink_orig)
	// );
	//
	// // Hook flashlight functions.
	// Log(INFO, true, "Hooking CPortal_Player::FlashlightTurnOn...");
	// MH_CreateHook(
	// 	Memory::Scan<void*>(MODULE_SERVER, "A1 ? ? ? ? 8B 50 ? 83 7A ? ? 75"),
	// 	&CPortal_Player__FlashlightTurnOn_hook, reinterpret_cast<void**>(&CPortal_Player__FlashlightTurnOn_orig)
	// );
	// Log(INFO, true, "Hooking CPortal_Player::FlashlightTurnOff...");
	// MH_CreateHook(
	// 	Memory::Scan<void*>(MODULE_SERVER, "A1 ? ? ? ? 8B 50 ? 83 7A ? ? 74 ? 8B 81"),
	// 	&CPortal_Player__FlashlightTurnOff_hook, reinterpret_cast<void**>(&CPortal_Player__FlashlightTurnOff_orig)
	// );
	
	// Stop workshop map downloads by not returning false on the download request.
	//!! TEMP TO FIX ORIGINAL HOOK THAT SUDDENLY BROKE
	// Log(INFO, true, "Hooking CWorkshopManager::CreateFileDownloadRequest...");
	// MH_CreateHook(
	// 	Memory::Scan<void*>(MODULE_CLIENT, "55 8B EC 81 EC 48 01 00 00 57"),
	// 	&CUGCFileRequestManager__Update_hook, reinterpret_cast<void**>(&CUGCFileRequestManager__Update_orig)
	// );

	// Log(INFO, true, "Hooking CEnvProjectedTexture::EnforceSingleProjectionRules...");
	// MH_CreateHook(
	// 	Memory::Scan<void*>(CLIENT, "55 8B EC 8B 45 ? 8B 55 ? 50 8B 45 ? 52 8B 55 ? 50 8B 45 ? 52 8B 55 ? 50 8B 45"),
	// 	&CWorkshopManager__CreateFileDownloadRequest_hook, reinterpret_cast<void**>(&CWorkshopManager__CreateFileDownloadRequest_orig)
	// );
	
	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}