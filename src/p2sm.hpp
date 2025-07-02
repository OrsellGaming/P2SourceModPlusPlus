//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: P2SourceModPlusPlus plugin
//
//===========================================================================//

#pragma once

#ifndef P2SM_HPP
#define P2SM_HPP

#include "interfaces/serverplugin.hpp"
#include "interfaces/gameevents.hpp"

#define P2SMPLUSPLUS_PLUGIN_VERSION "1.2.0 PREVIEW" // Update this when a new version of the plugin is released.

//---------------------------------------------------------------------------------
// Purpose: P2SourceModPlusPlus server plugin class
//---------------------------------------------------------------------------------

class CP2SMPlusPlusPlugin : public IServerPluginCallbacks, public IGameEventListener2
{
public:
	CP2SMPlusPlusPlugin();
	~CP2SMPlusPlusPlugin() override;

	bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) override;
	void			Unload() override;
	void			Pause() override;
	void			UnPause() override;
	const char*     GetPluginDescription() override;
	void			LevelInit(const char* mapName) override;
	void			ServerActivate(Edict* edictList, int edictCount, int clientMax) override;
	void			GameFrame(bool simulating) override;
	void			LevelShutdown() override;
	void			ClientActive(Edict* edict) override;
	void			ClientFullyConnect(Edict* edict) override;
	void			ClientDisconnect(Edict* edict) override;
	void			ClientPutInServer(Edict* edict, char const* playerName) override;
	void			SetCommandClient(int index) override;
	void			ClientSettingsChanged(Edict* edict) override;
	PluginResult	ClientConnect(bool* allowConnect, Edict* edict, const char* name, const char* ipAddress, char* reject, int maxRejectLen) override;
	PluginResult	ClientCommand(Edict* edict, const CCommand& args) override;
	PluginResult	NetworkIDValidated(const char* username, const char* networkID) override;
	void			OnQueryCvarValueFinished(QueryCvarCookie cookie, Edict* edict, QueryCvarValueStatus status, const char* cvarName, const char* cvarValue) override;
	void			OnEdictAllocated(Edict* edict) override;
	void			OnEdictFreed(const Edict* edict) override;

	// IGameEventListener2 methods
	virtual void			FireGameEvent(IGameEvent* event);
	virtual int				GetEventDebugID(void) { return debugID; }

	virtual int				GetCommandIndex() { return clientCommandIndex; }

private:
	// Plugin state member variables.
	bool		pluginLoaded;
	bool		noUnload;

	int			debugID;
	int			clientCommandIndex;
};

static CP2SMPlusPlusPlugin g_P2SMPlusPlusPlugin;

#endif
