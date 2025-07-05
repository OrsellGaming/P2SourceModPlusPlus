/*******************************************************************
* @file   cportal_player.cpp
* @brief  CPortal_Player class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cportal_player.hpp"

#include "modules/cbaseentity.hpp"
#include "modules/cbaseplayer.hpp"

#include "utils.hpp"
#include "commands.hpp"
#include "globals.hpp"

// Redeclaration's for hooks.
REDECL(CPortal_Player::PlayerDeathThink);
REDECL(CPortal_Player::FlashlightTurnOn);
REDECL(CPortal_Player::FlashlightTurnOff);

#pragma region CPortal_Player Hooks // MARK: Hooks

/**
 * @brief For hooking onto the function that is called before a player respawns to skip the delay that is usual there and instead force a instant respawn of the player.
 */
DEFINE_HOOK(CPortal_Player, PlayerDeathThink);
DETOUR_T(void, CPortal_Player::PlayerDeathThink)
{
	if (p2sm_multiplayer_instantrespawn.GetBool())
    {
    	CPortal_Player::RespawnPlayer(Utils::EntityIndex(static_cast<CBaseEntity*>(thisPtr)));
    	return;
    }

	h_PlayerDeathThink->Disable();
	CPortal_Player::PlayerDeathThink(thisPtr);
	h_PlayerDeathThink->Enable();
}

/**
 * @brief Player flashlight turn on hook.
 * @param playSound If flashlight sound should play. Coded to always pass as false.
 * @return If flashlight was enabled successfully.
 */
DEFINE_HOOK(CPortal_Player, FlashlightTurnOn);
DETOUR_T(bool, CPortal_Player::FlashlightTurnOn, bool playSound)
{
	CBaseEntity* pEntity = static_cast<CBaseEntity*>(thisPtr);
	const int playerIndex = Utils::EntityIndex(pEntity);
	if (playerIndex <= 0 || playerIndex > MAX_PLAYERS)
	{
		Log(WARNING, false, "CPortal_Player::FlashlightTurnOn was called with a invalid player!");
		return false;
	}

	CPortal_Player::SetFlashlightState(playerIndex, true);

	CPlayerFilter filter;
	filter.AddPlayer(playerIndex);
	CBaseEntity::EmitSound(pEntity, playerIndex, filter, "HL2Player.FlashLightOn", nullptr, 0.0f);
	return true;
}

/**
 * @brief Player flashlight turn off hook.
 * @param playSound If flashlight sound should play. Coded to always pass as false.
 */
DEFINE_HOOK(CPortal_Player, FlashlightTurnOff);
DETOUR_T(void, CPortal_Player::FlashlightTurnOff, bool playSound)
{
	CBaseEntity* pEntity = static_cast<CBaseEntity*>(thisPtr);
	const int playerIndex = Utils::EntityIndex(static_cast<CBaseEntity*>(thisPtr));
	if (playerIndex <= 0 || playerIndex > MAX_PLAYERS)
	{
		Log(WARNING, false, "CPortal_Player::FlashlightTurnOff was called with a invalid player!");
		return;
	}

	CPortal_Player::SetFlashlightState(playerIndex, false);

	CPlayerFilter filter;
	filter.AddPlayer(playerIndex);
	CBaseEntity::EmitSound(pEntity, playerIndex, filter, "HL2Player.FlashLightOff", nullptr, 0.0f);
}

#pragma endregion

#pragma region Functions // MARK: Functions

/**
 * @brief Respawn a player where ever their spawn point is set.
 * @param playerEntityIndex Player entity index.
 */
void CPortal_Player::RespawnPlayer(const int playerEntityIndex)
{
    const auto pPlayer = reinterpret_cast<CPortal_Player*>(Utils::PlayerByIndex(playerEntityIndex));
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
void CPortal_Player::SetFlashlightState(int playerEntityIndex, bool enabled)
{
	CBasePlayer* pPlayer = Utils::PlayerByIndex(playerEntityIndex);
	if (!pPlayer)
	{
		Log(WARNING, true, "Couldn't get player to set flashlight state! playerIndex: %i enable: %i", playerEntityIndex, !!enabled);
		return;
	}

	if (enabled)
		CBaseEntity::AddEffects(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
	else
		CBaseEntity::RemoveEffects(reinterpret_cast<CBaseEntity*>(pPlayer), EF_DIMLIGHT);
}

#pragma	endregion