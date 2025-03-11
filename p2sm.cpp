//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: P2SourceModPlusPlus plugin
// 
//===========================================================================//

#include "p2sm.hpp"
#include "scanner.hpp"

#include "cdll_int.h"
#include "minhook/include/MinHook.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void Log(int level, bool dev, const char* pMsgFormat, ...);
static ConVar p2sm_developer("p2sm_developer", "0", FCVAR_HIDDEN, "Enable for developer messages.");

//---------------------------------------------------------------------------------
// The plugin is a static singleton that is exported as an interface
//---------------------------------------------------------------------------------
static CP2SMPlusPlusPlugin g_P2SMPlusPlusPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CP2SMPlusPlusPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_P2SMPlusPlusPlugin);

//---------------------------------------------------------------------------------
// Purpose: Logging for the plugin by adding a prefix and line break.
// Max character limit of 1024 characters.	
// level:	0 = Msg/DevMsg, 1 = Warning/DevWarning, 2 = Error WILL STOP ENGINE!
//---------------------------------------------------------------------------------
void Log(int level, bool dev, const char* pMsgFormat, ...)
{
	if (dev && !p2sm_developer.GetBool() && level != 2) return; // Stop developer messages when p2mm_developer isn't enabled.

	// Take our log message and format any arguments it has into the message.
	va_list argPtr;
	char szFormattedText[1024] = { 0 };
	va_start(argPtr, pMsgFormat);
	V_vsnprintf(szFormattedText, sizeof(szFormattedText), pMsgFormat, argPtr);
	va_end(argPtr);

	// Add a header to the log message.
	char completeMsg[1024] = { 0 };
	V_snprintf(completeMsg, sizeof(completeMsg), "(P2SourceModPlusPlus PLUGIN): %s\n", szFormattedText);

	switch (level)
	{
	case 0:
		ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
		return;
	case 1:
		Warning(completeMsg);
		return;
	case 2:
		Warning("(P2SourceModPlusPlus PLUGIN):\n!!!ERROR ERROR ERROR!!!:\nA FATAL ERROR OCCURED WITH THE ENGINE:\n%s", completeMsg);
		Error(completeMsg);
		return;
	default:
		Warning("(P2SourceModPlusPlus PLUGIN): Log level set outside of 0-1, \"%i\". Defaulting to level 0.\n", level);
		ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
	}
}

//---------------------------------------------------------------------------------
// Purpose: constructor
//---------------------------------------------------------------------------------
CP2SMPlusPlusPlugin::CP2SMPlusPlusPlugin()
{
	this->m_bPluginLoaded = false;
	this->m_bNoUnload = false;	  // If we fail to load, we don't want to run anything on Unload() to get what the error was.
}

//---------------------------------------------------------------------------------
// Purpose: Description of plugin outputted when the "plugin_print" console command is executed.
//---------------------------------------------------------------------------------
const char* CP2SMPlusPlusPlugin::GetPluginDescription(void)
{
	return "P2SourceModPlusPlus Plugin | Plugin Version: " P2SMPLUSPLUS_PLUGIN_VERSION;
}

//---------------------------------------------------------------------------------
// Purpose: Stop the UGC manager from automatically download workshop maps.
//---------------------------------------------------------------------------------
class CUGCFileRequestManager;
static void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisPtr);
static void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr) { } // Simply do nothing so that nothing gets updated and therefore nothing gets downloaded.

//---------------------------------------------------------------------------------
// Purpose: Called when the plugin is loaded, initialization process.
//			Loads the interfaces we need from the engine and applies our patches.
//---------------------------------------------------------------------------------
bool CP2SMPlusPlusPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
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

	// cl_localnetworkbackdoor is causing NPCs to not move correctly thanks to Valve networking "optimizations".
	// Turn it off, nothing else should turn it back automatically while in game.
	if (ConVar* lnbCVar = g_pCVar->FindVar("cl_localnetworkbackdoor"))
		lnbCVar->SetValue(0);

	// Remove the cheat flag on r_drawscreenoverlay and enable it by default to allow 
	if (ConVar* screenCVar = g_pCVar->FindVar("r_drawscreenoverlay"))
	{
		screenCVar->RemoveFlags(FCVAR_CHEAT);
		screenCVar->SetValue(1);
	}
	
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
			&CUGCFileRequestManager__Update_hook, reinterpret_cast<void**>(&CUGCFileRequestManager__Update_orig)
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
void CP2SMPlusPlusPlugin::Unload(void)
{
	// If the plugin errors for some reason, prevent it from unloading.
	if (m_bNoUnload)
	{
		m_bNoUnload = false;
		return;
	}

	Log(0, false, "Unloading Plugin...");

	Log(0, true, "Disconnecting hooked functions and un-initializing MinHook...");
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	m_bPluginLoaded = false;
	Log(0, false, "Plugin unloaded! Goodbye!");
}

//---------------------------------------------------------------------------------
// Purpose: Unused callbacks
//---------------------------------------------------------------------------------
#pragma region UNUSED_CALLBACKS
void CP2SMPlusPlusPlugin::SetCommandClient(int index) {}
void CP2SMPlusPlusPlugin::ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) {}
void CP2SMPlusPlusPlugin::LevelInit(char const* pMapName) {}
PLUGIN_RESULT CP2SMPlusPlusPlugin::ClientCommand(edict_t* pEntity, const CCommand& args) { return PLUGIN_CONTINUE; }
void CP2SMPlusPlusPlugin::ClientActive(edict_t* pEntity) {}
void CP2SMPlusPlusPlugin::GameFrame(bool simulating) {}
void CP2SMPlusPlusPlugin::LevelShutdown(void) {}
void CP2SMPlusPlusPlugin::Pause(void) {}
void CP2SMPlusPlusPlugin::UnPause(void) {}
void CP2SMPlusPlusPlugin::ClientDisconnect(edict_t* pEntity) {}
void CP2SMPlusPlusPlugin::ClientFullyConnect(edict_t* pEntity) {}
void CP2SMPlusPlusPlugin::ClientPutInServer(edict_t* pEntity, char const* playerName) {}
void CP2SMPlusPlusPlugin::ClientSettingsChanged(edict_t* pEdict) {}
PLUGIN_RESULT CP2SMPlusPlusPlugin::ClientConnect(bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxRejectLen) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT CP2SMPlusPlusPlugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return PLUGIN_CONTINUE; }
void CP2SMPlusPlusPlugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue) {}
void CP2SMPlusPlusPlugin::OnEdictAllocated(edict_t* edict) {}
void CP2SMPlusPlusPlugin::OnEdictFreed(const edict_t* edict) {}
bool CP2SMPlusPlusPlugin::BNetworkCryptKeyCheckRequired(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, bool bClientWantsToUseCryptKey) { return false; }
bool CP2SMPlusPlusPlugin::BNetworkCryptKeyValidate(uint32 unFromIP, uint16 usFromPort, uint32 unAccountIdProvidedByClient, int nEncryptionKeyIndexFromClient, int numEncryptedBytesFromClient, byte* pbEncryptedBufferFromClient, byte* pbPlainTextKeyForNetchan) { return true; }
#pragma endregion