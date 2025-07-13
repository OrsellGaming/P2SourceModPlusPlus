/*******************************************************************
* @file   cbaseplayer.hpp
* @brief  CBasePlayer class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cbaseplayer.hpp"

#include "signatures.hpp"
#include "utils.hpp"

/**
 * @brief Show or hide viewports for players.
 * @param player Player entity.
 * @param name Viewport to show.
 * @param show Whether to show or hide the viewport.
 * @param data Any additional data that would go with certain viewports.
 */
using ShowViewPortPanelT = void (__thiscall*)(CBasePlayer*, const char*, bool, KeyValues*);
void CBasePlayer::ShowViewPortPanel(CBasePlayer* player, const char* name, const bool show, KeyValues* data)
{
    if (!player)
        return;

    static auto showViewPortPanel = Memory::Scan<ShowViewPortPanelT>(MODULE_SERVER, Signatures::ShowViewPortPanel);
    showViewPortPanel(player, name, show, data);
}