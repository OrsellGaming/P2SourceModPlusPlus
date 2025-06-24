/*******************************************************************
* @file   cportal_player.h
* @brief  CPortal_Player class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef CPORTAL_PLAYER_H
#define CPORTAL_PLAYER_H

class CPortal_Player
{
public: // MARK: CPortal_Player Public Members
#pragma region Public Members

    static void RespawnPlayer(int playerEntityIndex);
    static void SetFlashlightState(int playerEntityIndex, bool enabled);
    
#pragma endregion

private: // MARK: CPortal_Player Private Members
#pragma region Private Members
#pragma endregion
};

#endif