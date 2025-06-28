/*******************************************************************
* @file   misc.cpp
* @brief  One off or miscellaneous game classes that don't fit into their own module.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/misc.hpp"

#include "utils.hpp"

// Redeclaration's for hooks.
REDECL(CUGCFileRequestManager::Update);
REDECL(CPrecipitation::Spawn);

// TODO: Replace unstable hook with byte patch!!
/**
 * @brief Stop the UGC manager from automatically download workshop maps. Simply return doing nothing so that nothing gets updated and therefore nothing gets downloaded.
 * @warning This makes the game extremely unstable, most of the time crashing, if the plugin is unloaded while the game is running.
 */
DECL_HOOK(CUGCFileRequestManager::Update_Hook, CUGCFileRequestManager_Update);
DETOUR_T(void, CUGCFileRequestManager::Update)
{
    return;
}

/**
 * @brief func_precipitation entity brushes are bugged where they need to set to the world origin to function properly. 
 */
DECL_HOOK(CPrecipitation::Spawn_Hook, CPrecipitation_Spawn);
DETOUR_T(void, CPrecipitation::Spawn)
{
    h_CPrecipitation_Spawn.Disable();
    CPrecipitation::Spawn(thisPtr);
    h_CPrecipitation_Spawn.Enable();
    Utils::SetOrigin(static_cast<CBaseEntity*>(thisPtr), Vector(0,0,0), false);
}
