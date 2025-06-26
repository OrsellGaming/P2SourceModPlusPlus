//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: P2SourceModPlusPlus plugin
// 
//===========================================================================//

#pragma once

#ifndef P2SM_HPP
#define P2SM_HPP

#include "engine/iserverplugin.h"

#define P2SMPLUSPLUS_PLUGIN_VERSION "1.2.0 PREVIEW" // Update this when a new version of the plugin is released.

//---------------------------------------------------------------------------------
// Purpose: P2SourceModPlusPlus server plugin class
//---------------------------------------------------------------------------------

class CP2SMPlusPlusPlugin : public IServerPluginCallbacks
{
public:
	CP2SMPlusPlusPlugin();
	virtual ~CP2SMPlusPlusPlugin() = default;

	// IServerPluginCallbacks methods.
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void			Unload(void);
	virtual void			Pause(void);
	virtual void			UnPause(void);
	virtual const char*     GetPluginDescription(void);
	virtual void			LevelInit(char const* pMapName);
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
	virtual void			GameFrame(bool simulating);
	virtual void			LevelShutdown(void);
	virtual void			ClientActive(edict_t* pEntity);
	virtual void			ClientDisconnect(edict_t* pEntity);
	virtual void			ClientPutInServer(edict_t* pEntity, char const* playerName);
	virtual void			SetCommandClient(int index);
	virtual void			ClientSettingsChanged(edict_t* pEdict);
	virtual PLUGIN_RESULT	ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxRejectLen);
	virtual void			ClientFullyConnect(edict_t* pEntity);
	virtual PLUGIN_RESULT	ClientCommand(edict_t* pEntity, const CCommand& args);
	virtual PLUGIN_RESULT	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue);
	virtual void			OnEdictAllocated(edict_t* edict);
	virtual void			OnEdictFreed(const edict_t* edict);
	virtual bool			BNetworkCryptKeyCheckRequired(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, bool bClientWantsToUseCryptKey);
	virtual bool			BNetworkCryptKeyValidate(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, int nEncryptionKeyIndexFromClient, int numEncryptedBytesFromClient, byte* pbEncryptedBufferFromClient, byte* pbPlainTextKeyForNetchan);

private:
	// Plugin state member variables.
	bool		m_bPluginLoaded;
	bool		m_bNoUnload;
};

static CP2SMPlusPlusPlugin g_P2SMPlusPlusPlugin;

#endif
