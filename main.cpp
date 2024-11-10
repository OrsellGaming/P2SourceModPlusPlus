//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: WorkshopStopper9000 plugin
// 
//===========================================================================//

#include "main.hpp"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void Log(int level, bool dev, const char* pMsgFormat, ...);
ConVar wss9000_developer("wss9000_developer", "0", FCVAR_NONE, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// The plugin is a static singleton that is exported as an interface
//---------------------------------------------------------------------------------
CWSS9000Plugin g_WSS9000Plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CWSS9000Plugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_WSS9000Plugin);

//---------------------------------------------------------------------------------
// Purpose: Logging for the plugin by adding a prefix and line break.
// Max character limit of 1024 characters.	
// level:	0 = Msg/DevMsg, 1 = Warning/DevWarning
//---------------------------------------------------------------------------------
void Log(int level, bool dev, const char* pMsgFormat, ...)
{
	va_list argptr;
	char szFormattedText[1024];
	va_start(argptr, pMsgFormat);
	V_vsnprintf(szFormattedText, sizeof(szFormattedText), pMsgFormat, argptr);
	va_end(argptr);

	char completeMsg[1024];
	V_snprintf(completeMsg, sizeof(completeMsg), "(WorkshopStopper9000 PLUGIN): %s\n", szFormattedText);

	if (dev && !wss9000_developer.GetBool()) { return; }

	switch (level)
	{
	case 0:
		ConColorMsg(WSS9000_PLUGIN_CONSOLE_COLOR, completeMsg);
		return;
	case 1:
		Warning(completeMsg);
		return;
	default:
		Warning("(WorkshopStopper9000 PLUGIN): Log level set outside of 0-1, \"%i\", defaulting to ConColorMsg().\n", level);
		ConColorMsg(WSS9000_PLUGIN_CONSOLE_COLOR, completeMsg);
		return;
	}
}

//---------------------------------------------------------------------------------
// Purpose: constructor
//---------------------------------------------------------------------------------
CWSS9000Plugin::CWSS9000Plugin()
{
	this->m_bPluginLoaded = false;
	this->m_bNoUnload = false;	  // If we fail to load, we don't want to run anything on Unload() to get what the error was.
}

//---------------------------------------------------------------------------------
// Purpose: destructor
//---------------------------------------------------------------------------------
CWSS9000Plugin::~CWSS9000Plugin() {}

//---------------------------------------------------------------------------------
// Purpose: Description of plugin outputted when the "plugin_print" console command is executed.
//---------------------------------------------------------------------------------
const char* CWSS9000Plugin::GetPluginDescription(void)
{
	return "WorkshopStopper9000 Plugin | Plugin Version: " WSS9000_PLUGIN_VERSION;
}

// STOP THEM WORKSHOP DOWNLOADS!
class CUGCFileRequestManager;
void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisptr);
void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisptr) { return; }

//---------------------------------------------------------------------------------
// Purpose: Called when the plugin is loaded, initialization process.
//			Loads the interfaces we need from the engine and applies our patches.
//---------------------------------------------------------------------------------
bool CWSS9000Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	if (m_bPluginLoaded)
	{
		Log(1, false, "Already loaded!");
		m_bNoUnload = true;
		return false;
	}

	Log(0, false, "Loading plugin...");

	Log(0, true, "Connecting tier libraries...");
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
	ConVar_Register(0);
	
	// big ol' try catch because game has a TerminateProcess handler for exceptions...
	// why this wasn't here is mystifying, - 10/2024 NULLderef
	try {
		// MinHook initialization and hooking
		Log(0, true, "Initializing MinHook and hooking functions...");
		MH_Initialize();

		// STOP THEM WORKSHOP DOWNLOADS: MinHook Edition
#if _WIN32
		MH_CreateHook(
			Memory::Scanner::Scan(CLIENTDLL, "55 8B EC 81 EC 48 01 00 00 57"),
			&CUGCFileRequestManager__Update_hook, (void**)&CUGCFileRequestManager__Update_orig
		);
#else // Linux Hooking. Due to the way this plugin is structured, it's currently not possible to compile this for Linux. Literally 1984 I know, but I don't have enough time or experience to figure it out by myself.
#endif // _WIN32

		MH_EnableHook(MH_ALL_HOOKS);

		Log(0, false, "Loaded plugin! Hooray! :D");
		m_bPluginLoaded = true;
	} catch (const std::exception& ex) {
		Log(0, false, "Failed to load plugin! :( Exception: \"%s\"", ex.what());
		this->m_bNoUnload = true;
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------
// Purpose: Called when the plugin is turning off/unloading.
//---------------------------------------------------------------------------------
void CWSS9000Plugin::Unload(void)
{
	// If the plugin errors for some reason, prevent it from unloading.
	if (m_bNoUnload)
	{
		m_bNoUnload = false;
		return;
	}

	Log(0, false, "Unloading Plugin...");

	Log(0, true, "Disconnecting hooked functions and uninitializing MinHook...");
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	m_bPluginLoaded = false;
	Log(0, false, "Plugin unloaded! Goodbye!");
}

//---------------------------------------------------------------------------------
// Purpose: Unused callbacks
//---------------------------------------------------------------------------------
#pragma region UNUSED_CALLBACKS
void CWSS9000Plugin::SetCommandClient(int index) {}
void CWSS9000Plugin::ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {}
void CWSS9000Plugin::LevelInit(char const* pMapName) {}
PLUGIN_RESULT CWSS9000Plugin::ClientCommand(edict_t* pEntity, const CCommand& args) { return PLUGIN_CONTINUE; }
void CWSS9000Plugin::ClientActive(edict_t* pEntity) {}
void CWSS9000Plugin::GameFrame(bool simulating) {}
void CWSS9000Plugin::LevelShutdown(void) {}
void CWSS9000Plugin::Pause(void) {}
void CWSS9000Plugin::UnPause(void) {}
void CWSS9000Plugin::ClientDisconnect(edict_t* pEntity) {}
void CWSS9000Plugin::ClientFullyConnect(edict_t* pEntity) {} // Purpose: Called when a player is fully connected to the server. Player entity still has not spawned in so manipulation is not possible.
void CWSS9000Plugin::ClientPutInServer(edict_t* pEntity, char const* playername) {}
void CWSS9000Plugin::ClientSettingsChanged(edict_t* pEdict) {}
PLUGIN_RESULT CWSS9000Plugin::ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT CWSS9000Plugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return PLUGIN_CONTINUE; }
void CWSS9000Plugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) {}
void CWSS9000Plugin::OnEdictAllocated(edict_t* edict) {}
void CWSS9000Plugin::OnEdictFreed(const edict_t* edict) {}
bool CWSS9000Plugin::BNetworkCryptKeyCheckRequired(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, bool bClientWantsToUseCryptKey) { return false; }
bool CWSS9000Plugin::BNetworkCryptKeyValidate(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, int nEncryptionKeyIndexFromClient, int numEncryptedBytesFromClient, byte* pbEncryptedBufferFromClient, byte* pbPlainTextKeyForNetchan) { return true; }
#pragma endregion