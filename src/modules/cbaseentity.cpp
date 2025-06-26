/*******************************************************************
* @file   cbaseentity.cpp
* @brief  CBaseEntity class recreation.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cbaseentity.h"

#include "modules/cbaseplayer.h"

#include "utils/memory.hpp"
#include "utils/loggingsystem.hpp"

#include "globals.hpp"

#include "vscript/ivscript.h"
#include "irecipientfilter.h"

/**
 * @brief Remove a entity from the world.
 * @param pEntity Pointer to entity.
 */
void CBaseEntity::RemoveEntity(CBaseEntity* pEntity)
{
    if (!pEntity)
        return;
    
    static auto removeEntity = reinterpret_cast<void (__cdecl*)(void*)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 57 8B 7D 08 85 FF 74 72"));
    removeEntity((reinterpret_cast<IServerEntity*>(pEntity)->GetNetworkable()));
}

/**
 * @brief Get the script scope of a entity.
 * @param pEntity Pointer to entity.
 * @return VScript handle to entity's script scope.
 */
HSCRIPT CBaseEntity::GetScriptScope(CBaseEntity* pEntity)
{
    if (!pEntity)
        return nullptr;

    return *reinterpret_cast<HSCRIPT*>(reinterpret_cast<uintptr_t>(pEntity) + 0x33C);
}

/**
 * @brief Get the script instance of a entity.
 * @param entity Pointer to entity.
 * @return VScript instance handle of the entity.
 */
HSCRIPT CBaseEntity::GetScriptInstance(CBaseEntity* entity)
{
    static auto getScriptInstance = reinterpret_cast<HSCRIPT (__thiscall*)(CBaseEntity*)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 51 56 8B F1 83 BE 50"));
    if (!getScriptInstance)
    {
        Log(WARNING, false, "Could not get script instance for entity!");
        return nullptr;
    }

    return getScriptInstance(entity);
}

/**
 * @brief Make a entity emit a sound.
 * @param pEntity Entity that will call to emit a sound.
 * @param entityIndex Entity that will emit the sound. Use -1 if you want to set a position in the world for it to play.
 * @param filter Filter of recipient entities that can hear this noise.
 * @param soundName Sound file path, or script name to play.
 * @param pOrigin Position in the world the sound will play.
 * @param soundTime Time in seconds till sound is played. NOT HOW LONG SOUND WILL PLAY! 
 * @return Return code for sound.
 */
int CBaseEntity::EmitSound(CBaseEntity* pEntity, int entityIndex, IRecipientFilter* filter, const char* soundName, const Vector* pOrigin, const float soundTime)
{
    static auto emitSound = reinterpret_cast<int (__thiscall*)(CBaseEntity*, IRecipientFilter*, int, const char*, const Vector*, float)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 83 EC 4C 8B 0D"));
    return emitSound(pEntity, filter, entityIndex, soundName, pOrigin, soundTime);
}

void CBaseEntity::AddEffects(CBaseEntity* pEntity, int nEffects)
{
    static auto addEffects = reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 53 8B D9 8B 83 A8"));
    addEffects(pEntity, nEffects);
}

void CBaseEntity::RemoveEffects(CBaseEntity* pEntity, int nEffects)
{
    static auto removeEffects = reinterpret_cast<void(__thiscall*)(CBaseEntity*, int)>(Memory::Scan<void*>(MODULE_SERVER, "55 8B EC 53 56 8B 75 08 8B D9 8B 83"));
    removeEffects(pEntity, nEffects);
}
