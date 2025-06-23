/*******************************************************************
* @file   commands.cpp
* @brief  ConVars and ConCommands the plugin uses.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "commands.hpp"

#include "utils/loggingsystem.hpp"
#include "utils/memory.hpp"

//---------------------------------------------------------------------------------
// Debug P2:SM++ ConVars | Self-explanatory.
//---------------------------------------------------------------------------------
ConVar p2sm_developer("p2sm_developer", "0", FCVAR_NONE, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// UTIL P2:SM++ ConVars | ConVars the host can change.
//---------------------------------------------------------------------------------
ConVar p2sm_multiplayer_instantrespawn("p2sm_multiplayer_instantrespawn", "0", FCVAR_NONE, "Whether respawning should be instant or not in multiplayer.");

static void ChangePtexVolumetricState(IConVar* var, const char* pOldValue, float flOldValue)
{
    if (!dynamic_cast<ConVar*>(var)->GetBool())
    {
        Log(INFO, false, "Disabled volumetrics for projected textures...");
        Memory::ReplacePattern("engine", "0F 84 47 0C 00 00 80 BF 10 02 00 00 00 0F 85 3A 0C 00 00", "0F 85 47 0C 00 00 80 BF 10 02 00 00 00 0F 84 3A 0C 00 00");
    }
    else
    {
        Log(INFO, false, "Enabled volumetrics for projected textures...");
        Memory::ReplacePattern("engine", "0F 85 47 0C 00 00 80 BF 10 02 00 00 00 0F 84 3A 0C 00 00", "0F 84 47 0C 00 00 80 BF 10 02 00 00 00 0F 85 3A 0C 00 00");
    }
}
ConVar p2sm_ptex_volumetrics("p2sm_ptex_volumetrics", "0", FCVAR_NONE, "Enable or disable volumetrics for projected textures.", ChangePtexVolumetricState);

