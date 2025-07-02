/*******************************************************************
* @file   serverplugin.hpp
* @brief  Source Engine server plugin interface.
* @author Orsell
* @date   07 2025
*********************************************************************/

#pragma once

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

struct Edict;
class CCommand;

enum class PluginResult
{
    Continue = 0, // keep going
    Override,     // run the game dll function but use our return value instead
    Stop,         // don't run the game dll function at all
};

typedef int QueryCvarCookie;
enum class QueryCvarValueStatus
{
    ValueIntact     = 0, // It got the value fine.
    CvarNotFound,
    NotACvar,            // There's a ConCommand, but it's not a ConVar.
    CvarProtected	     // The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
};

class IServerPluginCallbacks
{
public:
    virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
    virtual void			Unload() = 0;
    virtual void			Pause() = 0;
    virtual void			UnPause() = 0;
    virtual const char*     GetPluginDescription() = 0;
    virtual void			LevelInit(const char* mapName) = 0;
    virtual void			ServerActivate(Edict* edictList, int edictCount, int clientMax) = 0;
    virtual void			GameFrame(bool simulating) = 0;
    virtual void			LevelShutdown() = 0;
    virtual void			ClientActive(Edict* edict) = 0;
    virtual void			ClientFullyConnect(Edict* edict) = 0;
    virtual void			ClientDisconnect(Edict* edict) = 0;
    virtual void			ClientPutInServer(Edict* edict, char const* playerName) = 0;
    virtual void			SetCommandClient(int index) = 0;
    virtual void			ClientSettingsChanged(Edict* edict) = 0;
    virtual PluginResult	ClientConnect(bool* allowConnect, Edict* edict, const char* name, const char* ipAddress, char* reject, int maxRejectLen) = 0;
    virtual PluginResult	ClientCommand(Edict* edict, const CCommand& args) = 0;
    virtual PluginResult	NetworkIDValidated(const char* username, const char* networkID) = 0;
    virtual void			OnQueryCvarValueFinished(QueryCvarCookie cookie, Edict* edict, QueryCvarValueStatus status, const char* cvarName, const char* cvarValue) = 0;
    virtual void			OnEdictAllocated(Edict* edict) = 0;
    virtual void			OnEdictFreed(const Edict* edict) = 0;
};
