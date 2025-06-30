/*******************************************************************
* @file   signatures.hpp
* @brief  Header to store all gather memory signatures or offsets used by the plugin.
*         This is based on SAR's Offsets.hpp, but because P2SM++ is designed to only
*         support the latest version of Portal 2, it's only the one file with signatures
*         and offsets.
* @author SAR Team, Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef SIGS_HPP
#define SIGS_HPP

#define OFFSET(name, win, linux) int (name);
#define SIGSCAN(name, win, linux) const char* (name);

namespace Signature
{
    // CPortal_Player::NotifySystemEvent | "movzx  eax, byte ptr [edi+504h]" -> "jmp  short loc_103552D0"
    SIGSCAN(CPortal_Player_NotifySystemEvent_EventTeleportUserID, "0F B6 87 04 05 00 00 8B 16", "")
    SIGSCAN(CPortal_Player_NotifySystemEvent_EventTeleportUserID_Patch, "EB 14 87 04 05 00 00 8B 16", "")

    // Squirrel VM runtime timeout, increase runtime max from 0.03 to 0.05.
    SIGSCAN(SQVM_RuntimeTimeout, "00 00 00 E0 51 B8 9E 3F", "")
    SIGSCAN(SQVM_RuntimeTimeout_Patch, "9A 99 99 99 99 99 A9 3F", "")

    // CEnvProjectedTexture::EnforceSingleProjectionRules | Skip the loop that goes through and disables all the env_projectedtextures
    SIGSCAN(CEnvProjectedTexture_EnforceSingleProjectionRules_Loop, "8B F0 3B F3 0F 84 95 00 00 00", "")
    SIGSCAN(CEnvProjectedTexture_EnforceSingleProjectionRules_Loop_Patch, "E9 9D 00 00 00 84 95 00 00 00", "")

    // CClientShadowMgr::CalculateRenderTargetsAndSizes | Increase the projected texture limit from 1 -> 8 
    SIGSCAN(CClientShadowMgr_CalculateRenderTargetsAndSizes_PtexLimit, "F7 D8 1B C0 83 E0 07 40 5F", "")
    SIGSCAN(CClientShadowMgr_CalculateRenderTargetsAndSizes_PtexLimit_Patch, "B8 08 00 00 00 90 90 90 5F", "")

    // CClientShadowMgr::CalculateRenderTargetsAndSizes | Increase the resolution shadow maps without tools mode.
    SIGSCAN(CClientShadowMgr_CalculateRenderTargetsAndSizes_ShadowRes, "0F 95 C3 89 8E", "")
    SIGSCAN(CClientShadowMgr_CalculateRenderTargetsAndSizes_ShadowRes_Patch, "0F 94 C3 89 8E", "")
    
    // CShadowMgr::DrawVolumetrics | Enable volumetrics for all env_projectedtextures.
    SIGSCAN(CShadowMgr_DrawVolumetrics_VolState, "0F 85 47 0C 00 00 80 BF 10 02 00 00 00 0F 84 3A 0C 00 00", "")
    SIGSCAN(CShadowMgr_DrawVolumetrics_VolState_Patch, "0F 84 47 0C 00 00 80 BF 10 02 00 00 00 0F 85 3A 0C 00 00", "")
}

#undef OFFSET
#undef SIGSCAN

#ifdef _WIN32

#define OFFSET(name, win, linux) name = win;
#define SIGSCAN(name, win, linux) name = win;

#else

#error "LINUX NOT IMPLEMENTED!!!"
#define OFFSET(name, win, linux) name = linux;
#define SIGSCAN(name, win, linux) name = linux;

#endif

#endif