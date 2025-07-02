/*******************************************************************
* @file   cbaseplayer.hpp
* @brief  CBasePlayer class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cbaseplayer.hpp"

#include "utils.hpp"

void CBasePlayer::ShowViewPortPanel(const int playerIndex, const char* name, const bool bShow, KeyValues* data)
{
    CBasePlayer* pPlayer = Utils::PlayerByIndex(playerIndex);
    if (!pPlayer)
    {
        Log(WARNING, false, "Couldn't get player to display view port panel to! playerIndex: %i", playerIndex);
        return;
    }
    static auto showViewPortPanel = reinterpret_cast<void(__thiscall*)(CBasePlayer*, const char*, bool, KeyValues*)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 83 EC 20 53 56 8B F1 57 8D 4D ? E8 ? ? ? ? 56"));
    showViewPortPanel(pPlayer, name, bShow, data);
}