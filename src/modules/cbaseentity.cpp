/*******************************************************************
* @file   cbaseentity.cpp
* @brief  CBaseEntity class recreation.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/cbaseentity.hpp"

#include "utils.hpp"
#include "globals.hpp"
#include "signatures.hpp"

/**
 * @brief Remove a entity from the world.
 * @param entity Pointer to entity.
 */
using RemoveEntityT = void (__cdecl*)(CBaseEntity*);
void CBaseEntity::RemoveEntity(CBaseEntity* entity)
{
    if (!entity)
        return;

    static auto removeEntity = Memory::Scan<RemoveEntityT>(MODULE_SERVER, Signatures::RemoveEntity);
    removeEntity(entity);
}

/**
 * @brief Get the script scope of a entity.
 * @param entity Pointer to entity.
 * @return VScript handle to entity's script scope.
 */
HSCRIPT CBaseEntity::GetScriptScope(CBaseEntity* entity)
{
    if (!entity)
        return nullptr;

    return *reinterpret_cast<HSCRIPT*>(reinterpret_cast<uintptr_t>(entity) + 0x33C);
}

/**
 * @brief Get the script instance of a entity.
 * @param entity Pointer to entity.
 * @return VScript instance handle of the entity.
 */
using GetScriptInstanceT = HSCRIPT (__thiscall*)(CBaseEntity*);
HSCRIPT CBaseEntity::GetScriptInstance(CBaseEntity* entity)
{
    if (!entity)
        return nullptr;

    static auto getScriptInstance = Memory::Scan<GetScriptInstanceT>(MODULE_SERVER, Signatures::GetScriptInstance);
    if (!getScriptInstance)
    {
        Log(WARNING, false, "Could not get script instance for entity!");
        return nullptr;
    }

    return getScriptInstance(entity);
}

/**
 * @brief Make a entity emit a sound.
 * @param entity Entity that will call to emit a sound.
 * @param entityIndex Entity that will emit the sound. Use -1 if you want to set a position in the world for it to play.
 * @param filter Filter of recipient entities that can hear this noise.
 * @param soundName Sound file path, or script name to play.
 * @param origin Position in the world the sound will play.
 * @param soundTime Time in seconds till sound is played. NOT HOW LONG SOUND WILL PLAY!
 * @return Return code for sound.
 */
using EmitSoundT = int (__thiscall*)(CBaseEntity*, IRecipientFilter&, int, const char*, const Vector*, float);
int CBaseEntity::EmitSound(CBaseEntity* entity, const int entityIndex, IRecipientFilter& filter, const char* soundName, const Vector* origin, const float soundTime)
{
    if (!entity)
        return 0;

    static auto emitSound = Memory::Scan<EmitSoundT>(MODULE_SERVER, Signatures::EmitSound);
    return emitSound(entity, filter, entityIndex, soundName, origin, soundTime);
}

/**
 * @brief Add effects to a entity.
 * @param entity Entity to add effects to.
 * @param effects Effects to apply on entity.
 */
using AddEffectsT = void (__thiscall*)(CBaseEntity*, int);
void CBaseEntity::AddEffects(CBaseEntity* entity, const EntityEffect effects)
{
    if (!entity)
        return;

    static auto addEffects = Memory::Scan<AddEffectsT>(MODULE_SERVER, Signatures::AddEffects);
    addEffects(entity, effects);
}

/**
 * @brief Remove effects from a entity.
 * @param entity Entity to remove effects from.
 * @param effects Effects to remove from entity.
 */
using RemoveEffectsT = void (__thiscall*)(CBaseEntity*, int);
void CBaseEntity::RemoveEffects(CBaseEntity* entity, const EntityEffect effects)
{
    if (!entity)
        return;

    static auto removeEffects = Memory::Scan<RemoveEffectsT>(MODULE_SERVER, Signatures::RemoveEffects);
    removeEffects(entity, effects);
}
