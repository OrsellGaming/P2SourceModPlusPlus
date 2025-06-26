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

// Respawn Hooks.
extern void (__fastcall* CPortal_Player__PlayerDeathThink_orig)(CPortal_Player* thisPtr);
void __fastcall CPortal_Player__PlayerDeathThink_hook(CPortal_Player* thisPtr);

// Flashlight Hooks.
extern bool (__fastcall* CPortal_Player__FlashlightTurnOn_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
bool __fastcall CPortal_Player__FlashlightTurnOn_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);
extern void (__fastcall* CPortal_Player__FlashlightTurnOff_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
void  __fastcall CPortal_Player__FlashlightTurnOff_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);

// Workshop download stopping hooks.
class CUGCFileRequestManager;
extern void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisptr);
void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr);