/*******************************************************************
* @file   misc.hpp
* @brief  One off or miscellaneous game classes that don't fit into their own module.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef MISC_HPP
#define MISC_HPP

#include "hook.hpp"
#include "platform.hpp"

class Hook;

class CUGCFileRequestManager
{
public: // MARK: CUGCFileRequestManager Public Members
#pragma region Public Members

    // Workshop download stopping hooks.
    DECL_HOOK(Update);
    DECL_DETOUR_T(void, Update);

#pragma endregion

private: // MARK: CUGCFileRequestManager Private Members
#pragma region Private Members
#pragma endregion
};

class CPrecipitation
{
public: // MARK: CPrecipitation Public Members
#pragma region Public Members

    // func_precipitation origin fix hook.
    DECL_HOOK(Spawn);
    DECL_DETOUR_T(void, Spawn);

#pragma endregion

private: // MARK: CPrecipitation Private Members
#pragma region Private Members
#pragma endregion
};


#endif
