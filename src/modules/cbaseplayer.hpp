/*******************************************************************
* @file   cbaseplayer.hpp
* @brief  CBasePlayer class recreation.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef CBASEPLAYER_HPP
#define CBASEPLAYER_HPP

class KeyValues;

class CBasePlayer
{
public: // MARK: CBasePlayer Public Members
#pragma region Public Members

    static void ShowViewPortPanel(int playerIndex, const char* name, bool bShow, KeyValues* data);
    
#pragma endregion

private: // MARK: CBasePlayer Private Members
#pragma region Private Members
#pragma endregion
};

#endif