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

// Entity effects used with AddEffects and RemoveEffects.
enum EntityEffect : std::uint16_t
{
    EF_BONEMERGE			        = 0x001,	// Performs bone merge on client side
    EF_BRIGHTLIGHT 			        = 0x002,	// DLIGHT centered at entity origin
    EF_DIMLIGHT 			        = 0x004,	// player flashlight
    EF_NOINTERP				        = 0x008,	// don't interpolate the next frame
    EF_NOSHADOW				        = 0x010,	// Don't cast no shadow
    EF_NODRAW				        = 0x020,	// don't draw entity
    EF_NORECEIVESHADOW		        = 0x040,	// Don't receive no shadow
    EF_BONEMERGE_FASTCULL	        = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
                                                // parent and uses the parent's bbox + the max extents of the aiment.
                                                // Otherwise, it sets up the parent's bones every frame to figure out where to place
                                                // the aiment, which is inefficient because it'll setup the parent's bones even if
                                                // the parent is not in the PVS.
    EF_ITEM_BLINK			        = 0x100,	// blink an item so that the user notices it.
    EF_PARENT_ANIMATES		        = 0x200,	// always assume that the parent entity is animating
    EF_MARKED_FOR_FAST_REFLECTION	= 0x400,	// marks an entity for reflection rendering when using $reflectonlymarkedentities material variable
    EF_NOSHADOWDEPTH		        = 0x800,	// Indicates this entity does not render into any shadow depthmap
    EF_SHADOWDEPTH_NOCACHE	        = 0x1000,	// Indicates this entity cannot be cached in shadow depthmap and should render every frame
    EF_NOFLASHLIGHT                 = 0x2000,
    EF_NOCSM				        = 0x4000,	// Indicates this entity does not render into the cascade shadow depthmap
    EF_MAX_BITS = 15
};

class CBaseEntity
{
public: // MARK: CBaseEntity Public Members
#pragma region Public Members

    static void     RemoveEntity(CBaseEntity* entity);
    static HSCRIPT  GetScriptScope(CBaseEntity* entity);
    static HSCRIPT  GetScriptInstance(CBaseEntity* entity);
    static int      EmitSound(CBaseEntity* entity, int entityIndex, IRecipientFilter& filter, const char* soundName, const Vector* origin, float soundTime);
    static void     AddEffects(CBaseEntity* entity, EntityEffect effects);
    static void     RemoveEffects(CBaseEntity* entity, EntityEffect effects);

#pragma endregion

private: // MARK: CBaseEntity Private Members
#pragma region Private Members
#pragma endregion
};

#endif
