//===========================================================================//
//
// Author: Orsell
// Purpose: Interfaced functions and hooks from the Portal 2 engine for the plugin to use.
// 
//===========================================================================//

#pragma once

//---------------------------------------------------------------------------------
// Hooked game functions.
//---------------------------------------------------------------------------------

// Workshop download stopping hooks.
class CUGCFileRequestManager;
extern void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisPtr);
void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr);