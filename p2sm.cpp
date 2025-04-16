//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: P2SourceMod++ Plugin
// 
//===========================================================================//

#include "p2sm.hpp"
#include "globals.hpp"
#include "scanner.hpp" // Memory scanner

#include "cdll_int.h" // Client interfacing
#include "eiface.h" // Server interfacing
#include "minhook/include/MinHook.h" // MinHook

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//---------------------------------------------------------------------------------
// The plugin is a static singleton that is exported as an interface
//---------------------------------------------------------------------------------
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CP2SMPlusPlusPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_P2SMPlusPlusPlugin);

//---------------------------------------------------------------------------------
// Purpose: constructor
//---------------------------------------------------------------------------------
CP2SMPlusPlusPlugin::CP2SMPlusPlusPlugin()
{
	hWnd = nullptr;
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
//			Simply do nothing so that nothing gets updated and therefore nothing gets downloaded.
//			!WARNING! This makes the game extremely unstable if the plugin is unloaded while the game is running.
//---------------------------------------------------------------------------------
static void (__fastcall* CUGCFileRequestManager__Update_orig)(CUGCFileRequestManager* thisPtr);
static void  __fastcall CUGCFileRequestManager__Update_hook(CUGCFileRequestManager* thisPtr) { } 

//---------------------------------------------------------------------------------
// Purpose: Called when the plugin is loaded, initialization process.
//			Loads the interfaces we need from the engine and applies our patches.
//---------------------------------------------------------------------------------
bool CP2SMPlusPlusPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	if (m_bPluginLoaded)
	{
		Log(WARNING, false, "Already loaded!");
		m_bNoUnload = true;
		return false;
	}

	Log(INFO, false,
		"\n"\
		R"( /$$$$$$$   /$$$$$$   /$$$$$$  /$$      /$$                    )""\n"\
		R"(| $$__  $$ /$$__  $$ /$$__  $$| $$$    /$$$    /$$       /$$   )""\n"\
		R"(| $$  \ $$|__/  \ $$| $$  \__/| $$$$  /$$$$   | $$      | $$   )""\n"\
		R"(| $$$$$$$/  /$$$$$$/|  $$$$$$ | $$ $$/$$ $$ /$$$$$$$$ /$$$$$$$$)""\n"\
		R"(| $$____/  /$$____/  \____  $$| $$  $$$| $$|__  $$__/|__  $$__/)""\n"\
		R"(| $$      | $$       /$$  \ $$| $$\  $ | $$   | $$      | $$   )""\n"\
		R"(| $$      | $$$$$$$$|  $$$$$$/| $$ \/  | $$   |__/      |__/   )""\n"\
		R"(|__/      |________/ \______/ |__/     |__/                    )""\n"\
		"(========================== VERSION: %s ==========================)", P2SMPLUSPLUS_PLUGIN_VERSION
	);
	
	Log(INFO, false, "Loading plugin...");

	Log(INFO, true, "Grabbing game window handle...");
	hWnd = FindWindow("Valve001", nullptr);
	if (!hWnd)
		Log(WARNING, false, "Failed to find game window!");
	
	Log(INFO, true, "Connecting tier libraries...");
	MathLib_Init(2.2f, 2.2f, 0.0f, 2.0f);
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);

	Log(INFO, true, "Registering plugin ConVars and ConCommands...");
	ConVar_Register(0);

	// Make sure that all the interfaces needed are loaded and usable.
	Log(INFO, true, "Loading interfaces...");
	Log(INFO, true, "Loading engineServer...");
	engineServer = static_cast<IVEngineServer*>(interfaceFactory(INTERFACEVERSION_VENGINESERVER, 0));
	if (!engineServer)
	{
		assert(0 && "Unable to load engineServer!");
		Log(WARNING, false, "Unable to load engineServer!");
		this->m_bNoUnload = true;
		return false;
	}

	Log(INFO, true, "Loading engineClient...");
	engineClient = static_cast<IVEngineClient*>(interfaceFactory(VENGINE_CLIENT_INTERFACE_VERSION, 0));
	if (!engineClient)
	{
		assert(0 && "Unable to load engineClient!");
		Log(WARNING, false, "Unable to load engineClient!");
		this->m_bNoUnload = true;
		return false;
	}

	Log(INFO, true, "Loading g_pPlayerInfoManager...");
	g_pPlayerInfoManager = static_cast<IPlayerInfoManager*>(gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, 0));
	if (!g_pPlayerInfoManager)
	{
		assert(0 && "Unable to load g_pPlayerInfoManager!");
		Log(WARNING, false, "Unable to load g_pPlayerInfoManager!");
		this->m_bNoUnload = true;
		return false;
	}
	
	Log(INFO, true, "Loading g_pGlobals...");
	g_pGlobals = g_pPlayerInfoManager->GetGlobalVars();
	if (!g_pGlobals)
	{
		assert(0 && "Unable to load g_pGlobals!");
		Log(WARNING, false, "Unable to load g_pGlobals!");
		this->m_bNoUnload = true;
		return false;
	}

	Log(INFO, true, "Updating/Fixing game ConVars and ConCommands...");

	// cl_localnetworkbackdoor is causing NPCs to not move correctly thanks to Valve networking "optimizations".
	// Turn it off, nothing else should turn it back automatically while in game.
	Log(INFO, true, "cl_localnetworkbackdoor...");
	if (ConVar* lnbCVar = g_pCVar->FindVar("cl_localnetworkbackdoor"))
		lnbCVar->SetValue(0);
	
	// Remove the cheat flag on r_drawscreenoverlay and enable it by default to allow maps to easily display screen overlays.
	Log(INFO, true, "r_drawscreenoverlay...");
	if (ConVar* screenCVar = g_pCVar->FindVar("r_drawscreenoverlay"))
	{
		screenCVar->RemoveFlags(FCVAR_CHEAT);
		screenCVar->SetValue(1);
	}
	
	// Make switching between players in splitscreen when testing easier by removing
	// the need for cheats to change the current player under control.
	Log(INFO, true, "in_forceuser...");
	if (ConVar* ifuCVar = g_pCVar->FindVar("in_forceuser"))
		ifuCVar->RemoveFlags(FCVAR_CHEAT);
	
	// big ol' try catch because game has a TerminateProcess handler for exceptions...
	// why this wasn't here is mystifying, - 10/2024 NULLderef
	try {
		// MinHook initialization and hooking
		Log(INFO, true, "Initializing MinHook and hooking functions...");
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

		Log(INFO, false, "Loaded plugin! Yay! :D");
		m_bPluginLoaded = true;
	} catch (const std::exception& ex) {
		Log(INFO, false, "Failed to load plugin! :( Exception: \"%s\"", ex.what());
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
		MessageBox(hWnd, "P2SM++ ran into a error when starting!\nPlease check the console for more info!", "P2SM++ Startup Error", MB_OK | MB_ICONERROR);
		return;
	}

	Log(INFO, false, "Unloading Plugin...");

	Log(INFO, true, "Disconnecting tier libraries...");
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();

	m_bPluginLoaded = false;
	Log(INFO, false, "Plugin unloaded! Goodbye!");
}

void CP2SMPlusPlusPlugin::ClientFullyConnect(edict_t* pEntity)
{
	// Make sure the r_drawscreenoverlay ConVar is enabled for connecting clients.
	engineServer->ClientCommand(pEntity, "r_drawscreenoverlay 1");
	Log(WARNING, true, "r_drawscreenoverlay set!");
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