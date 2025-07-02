/*******************************************************************
* @file   cbaseentity.hpp
* @brief  CBaseEntity class recreation.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef CBASEENTITY_HPP
#define CBASEENTITY_HPP

class Vector;
class IRecipientFilter;
class CBasePlayer;
struct Edict;
typedef struct HSCRIPT__* HSCRIPT;

class CBaseEntity
{
public: // MARK: CBaseEntity Public Members
#pragma region Public Members

    static void RemoveEntity(CBaseEntity* pEntity);
    static HSCRIPT GetScriptScope(CBaseEntity* pEntity);
    static HSCRIPT GetScriptInstance(CBaseEntity* entity);
    static int EmitSound(CBaseEntity* pEntity, int entityIndex, IRecipientFilter& filter, const char* soundName, const Vector* pOrigin, float soundTime);
    static void AddEffects(CBaseEntity* pEntity, int nEffects);
    static void RemoveEffects(CBaseEntity* pEntity, int nEffects);

#pragma endregion

private: // MARK: CBaseEntity Private Members
#pragma region Private Members
#pragma endregion
};

#endif
