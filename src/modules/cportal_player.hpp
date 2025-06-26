/*******************************************************************
* @file   cportal_player.hpp
* @brief  CPortal_Player class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef CPORTAL_PLAYER_HPP
#define CPORTAL_PLAYER_HPP

#include "utils/platform.hpp"

// Respawn Hooks.
// extern void (__fastcall* CPortal_Player__PlayerDeathThink_orig)(CPortal_Player* thisPtr);
// void __fastcall CPortal_Player__PlayerDeathThink_hook(CPortal_Player* thisPtr);

// Flashlight Hooks.
// extern bool (__fastcall* CPortal_Player__FlashlightTurnOn_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
// bool __fastcall CPortal_Player__FlashlightTurnOn_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);
// extern void (__fastcall* CPortal_Player__FlashlightTurnOff_orig)(CPortal_Player* thisPtr,  void* edx, bool playSound);
// void  __fastcall CPortal_Player__FlashlightTurnOff_hook(CPortal_Player* thisPtr,  void* edx, bool playSound);

class CPortal_Player
{
public: // MARK: CPortal_Player Public Members
#pragma region Public Members

    DECL_DETOUR_T(void, PlayerDeathThink);
    // using _PlayerDeathThink = void(__thiscall*)(void* thisPtr);
    // static _PlayerDeathThink PlayerDeathThink;
    // static void __fastcall PlayerDeathThink_Hook(void* thisPtr, int edx);
    
    DECL_DETOUR_T(bool, FlashlightTurnOn, bool playSound);
    // using _FlashlightTurnOn = bool(__thiscall*)(void* thisPtr, bool playSound);
    // static _FlashlightTurnOn FlashlightTurnOn;
    // static bool __fastcall FlashlightTurnOn_Hook(void* thisPtr, int edx, bool playSound);

    DECL_DETOUR_T(void, FlashlightTurnOff, bool playSound);
    // using _FlashlightTurnOff = void(__thiscall*)(void* thisPtr, bool playSound);
    // static _FlashlightTurnOff FlashlightTurnOff;
    // static void __fastcall FlashlightTurnOff_Hook(void* thisPtr, int edx, bool playSound);
    
    static void RespawnPlayer(int playerEntityIndex);
    static void SetFlashlightState(int playerEntityIndex, bool enabled);
    
#pragma endregion

private: // MARK: CPortal_Player Private Members
#pragma region Private Members
#pragma endregion
};

#endif