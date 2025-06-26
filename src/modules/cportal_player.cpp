/*******************************************************************
* @file   cportal_player.cpp
* @brief  CPortal_Player class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cportal_player.h"

#include <const.h>

#include "utils/loggingsystem.hpp"
#include "utils/memory.hpp"

#include "modules/cbaseplayer.h"
#include "modules/utils.h"

/**
 * @brief Respawn a player where ever their spawn point is set.
 * @param playerEntityIndex Player entity index.
 */
void CPortal_Player::RespawnPlayer(const int playerEntityIndex)
{
    CPortal_Player* pPlayer = reinterpret_cast<CPortal_Player*>(Utils::PlayerByIndex(playerEntityIndex));
    if (!pPlayer)
    {
        Log(WARNING, false, "Couldn't get player to respawn! playerIndex: %i", playerEntityIndex);
        return;
    }

    static auto respawnPlayer = reinterpret_cast<void(__thiscall*)(CPortal_Player*)>(Memory::Scan<void*>(MODULE_SERVER, "0F 57 C0 56 8B F1 57 8D 8E"));
    respawnPlayer(pPlayer);
}

/**
 * @brief Set the flashlight state for a player.
 * @note  Not a function in the CPortal_Player class, just more grouping it with the
 *        class. This does the same thing as the FlashlightTurnOn and FlashlightTurnOff
 *        functions in CPortal_Player but done in one function.
 * @param playerEntityIndex Player entity index.
 * @param enabled Should flashlight be enabled or not.
 */
void CPortal_Player::SetFlashlightState(const int playerEntityIndex, const bool enabled)
{
    CBasePlayer* pPlayer = Utils::PlayerByIndex(playerEntityIndex);
    if (!pPlayer)
    {
        Log(WARNING, true, "Couldn't get player to set flashlight state! playerIndex: %i enable: %i", playerEntityIndex, !!enabled);
        return;
    }

    if (enabled)
        reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 53 8B D9 8B 83 A8"))(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
    else
        reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 53 56 8B 75 08 8B D9 8B 83"))(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
}
