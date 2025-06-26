/*******************************************************************
* @file   cbaseentity.h
* @brief  CBaseEntity class recreation.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef CBASEENTITY_H
#define CBASEENTITY_H

class Vector;
class IRecipientFilter;
class CBasePlayer;
typedef struct HSCRIPT__* HSCRIPT;

class CBaseEntity
{
public: // MARK: CBaseEntity Public Members
#pragma region Public Members

    static void RemoveEntity(CBaseEntity* pEntity);
    static HSCRIPT GetScriptScope(CBaseEntity* pEntity);
    static HSCRIPT GetScriptInstance(CBaseEntity* entity);
    static int EmitSound(CBaseEntity* pEntity, int entityIndex, IRecipientFilter* filter, const char* soundName, const Vector* pOrigin, float soundTime);
    
#pragma endregion

private: // MARK: CBaseEntity Private Members
#pragma region Private Members
#pragma endregion
};

#endif
