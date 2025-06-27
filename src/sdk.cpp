//===========================================================================//
//
// Author: Orsell
// Purpose: Interfaced functions and hooks from the Portal 2 engine for the plugin to use.
// 
//===========================================================================//

#include "stdafx.hpp"
#include "sdk.hpp"

#include "utils.hpp"
#include "commands.hpp"
#include "p2sm.hpp"

// TODO: Replace unstable hook with byte patch!!
/**
 * @brief Stop the UGC manager from automatically download workshop maps. Simply return doing nothing so that nothing gets updated and therefore nothing gets downloaded.
 * @warning This makes the game extremely unstable if the plugin is unloaded while the game is running.
 */
void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisPtr);
void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr) { }

}
{
}