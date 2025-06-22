/*******************************************************************
* @file   commands.cpp
* @brief  ConVars and ConCommands the plugin uses.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "commands.hpp"

//---------------------------------------------------------------------------------
// Debug P2:SM++ ConVars | Self-explanatory.
//---------------------------------------------------------------------------------
ConVar p2sm_developer("p2sm_developer", "0", FCVAR_NONE, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// UTIL P2:SM++ ConVars | ConVars the host can change.
//---------------------------------------------------------------------------------
ConVar p2sm_instantrespawn("p2sm_instantrespawn", "0", FCVAR_NONE, "Whether respawning should be instant or not in multiplayer.");