/*******************************************************************
* @file   commands.cpp
* @brief  ConVars and ConCommands the plugin uses.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "commands.hpp"

#include "signatures.hpp"
#include "utils.hpp"

//---------------------------------------------------------------------------------
// Debug P2:SM++ ConVars | Self-explanatory.
//---------------------------------------------------------------------------------
ConVar p2sm_developer("p2sm_developer", "0", FCVAR_NONE, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// UTIL P2:SM++ ConVars | ConVars the host can change.
//---------------------------------------------------------------------------------
ConVar p2sm_multiplayer_instantrespawn("p2sm_multiplayer_instantrespawn", "0", FCVAR_NONE, "Whether respawning should be instant or not in multiplayer.");

static void ChangePtexVolumetricState(IConVar* var, const char* pOldValue, float flOldValue)
{
    if (dynamic_cast<ConVar*>(var)->GetBool())
    {
        Log(INFO, false, "Enabled volumetrics for projected textures...");
        Memory::ReplacePattern("engine", Signatures::CShadowMgr_DrawVolumetrics_VolState, Signatures::CShadowMgr_DrawVolumetrics_VolState_Patch);
    }
    else
    {
        Log(INFO, false, "Disabled volumetrics for projected textures...");
        Memory::ReplacePattern("engine", Signatures::CShadowMgr_DrawVolumetrics_VolState_Patch, Signatures::CShadowMgr_DrawVolumetrics_VolState);
    }
}
ConVar p2sm_ptex_volumetrics("p2sm_ptex_volumetrics", "0", FCVAR_NONE, "Enable or disable volumetrics for projected textures.", ChangePtexVolumetricState);

struct ChapterContext_t
{
    const char* mapName;
    int			chapterNum;
    int			subChapterNum;
};

static ChapterContext_t newChapters[] =
{
    {"testtesttest", 1, 2},
    {nullptr, 0, 0 }
};
CON_COMMAND(p2sm_spmaplist, "")
{
    // BF F0 2D 94 10 84 DB
    // GetNumChapters: 55 8B EC 80 7D ?? ?? 57 74

    //uintptr_t GetNumChapters = *static_cast<uintptr_t*>(Memory::Scanner::Scan<void*>(CLIENT, "55 8B EC 80 7D 08 00 57 74 0C", 0));//16));
    ChapterContext_t** chapterArrayPtr = Memory::Scan<ChapterContext_t**>(MODULE_CLIENT, "55 8B EC 80 7D 08 00 57 74 0C", 16);
    const ChapterContext_t* chapterArray = *chapterArrayPtr;
    // chapterArray = *reinterpret_cast<ChapterContext_t*>(tempChapterArray + 16);
    //chapterArray = reinterpret_cast<ChapterContext_t*>(GetNumChapters + 16);
    //chapterArray = static_cast<ChapterContext_t*>(Memory::Scanner::Scan<void*>(CLIENT, "B8 F0 A8 93 10 8D 64 24", 0));


    if (!chapterArrayPtr)
    {
        Log(WARNING, false, "Failed to get chapterArray!");
        return;
    }

    for (int i = 0; chapterArray[i].mapName != nullptr; i++)
    {
        Log(INFO, false, "Map: %s, Chapter: %d, Subchapter: %d\n",
            chapterArray[i].mapName,
            chapterArray[i].chapterNum,
            chapterArray[i].subChapterNum);
    }

    // Change it
    Memory::UnProtect(static_cast<void*>(chapterArrayPtr), sizeof(uintptr_t));

    *chapterArrayPtr = newChapters;


}
