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
DEFINE_HOOK(CUGCFileRequestManager, Update);
DETOUR_T(void, CUGCFileRequestManager::Update)
{
    return;
}

/**
 * @brief func_precipitation entity brushes are bugged where they need to set to the world origin to function properly.
 */
DEFINE_HOOK(CPrecipitation, Spawn);
DETOUR_T(void, CPrecipitation::Spawn)
{
    h_Spawn->Disable();
    CPrecipitation::Spawn(thisPtr);
    h_Spawn->Enable();
    Utils::SetOrigin(static_cast<CBaseEntity*>(thisPtr), Vector(0,0,0), false);
}
