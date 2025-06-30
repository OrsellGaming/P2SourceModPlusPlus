/*******************************************************************
* @file   patch-hook.hpp
* @brief  Perform hooks and patches for the plugin.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef PATCHHOOK_HPP
#define PATCHHOOK_HPP

namespace Patches
{
    bool EnablePatches();
    bool DisablePatches();
}

namespace Hooks
{
    bool EnableHooks();
    bool DisableHooks();
}

#endif

