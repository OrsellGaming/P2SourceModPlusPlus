//===========================================================================//
//
// Authors: Orsell & Nanoman2525 & NULLderef
// Purpose: P2SourceMod++ Plugin
//
//===========================================================================//

#include "stdafx.hpp"
#include "p2sm.hpp"

#include "modules/gui.hpp"

#include "utils.hpp"
#include "globals.hpp"
#include "patch-hook.hpp"

/**
 * @brief The plugin is a static singleton that is exported as an interface. Highest plugin interface version Portal 2 uses is version 3.
 */
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CP2SMPlusPlusPlugin, IServerPluginCallbacks, "ISERVERPLUGINCALLBACKS003", g_P2SMPlusPlusPlugin);

/**
 * @brief Plugin class constructor.
 */
CP2SMPlusPlusPlugin::CP2SMPlusPlusPlugin()
{
	this->pluginLoaded = false;
	this->noUnload = false; // If we fail to load, we don't want to run anything on Unload() to get what the error was.

	this->clientCommandIndex = 0;
	this->debugID = EVENT_DEBUG_ID_INIT;
}

/**
 * @brief Plugin class destructor.
 */
CP2SMPlusPlusPlugin::~CP2SMPlusPlusPlugin()
{
	this->debugID = EVENT_DEBUG_ID_SHUTDOWN;
}

/**
 * @brief Called when the plugin is loaded, the primary initialization process for the plugin.
 * @param interfaceFactory Interface factory to get game interfaces.
 * @param gameServerFactory Game server interface factory to get other game interfaces.
 * @return True if plugin started correctly.
 */
bool CP2SMPlusPlusPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	if (pluginLoaded)
	{
		Log(WARNING, false, "Already loaded!");
		noUnload = true;
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

	Log(INFO, true, "Connecting tier libraries and registering plugin ConVars and ConCommands...");
	MathLib_Init(2.2f, 2.2f, 0.0f, 2.0f);
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
	ConVar_Register(0);

	Log(INFO, true, "Initializing plugin GUI systems...");
	if (!GeneralGUI::InitializeGUISystems())
	{
		assert(0 && "Failed to initialize plugin GUI systems!");
		Log(WARNING, false, "Failed to initialize plugin GUI systems!");
	}

	// Make sure that all the interfaces needed are loaded and usable.
	Log(INFO, true, "Loading interfaces...");
	Log(INFO, true, "Loading engineServer...");
	engineServer = static_cast<IVEngineServer*>(interfaceFactory(INTERFACEVERSION_VENGINESERVER, nullptr));
	if (!engineServer)
	{
		assert(false && "Unable to load engineServer!");
		Log(WARNING, false, "Unable to load engineServer!");
		this->noUnload = true;
		return false;
	}

	Log(INFO, true, "Loading engineClient...");
	engineClient = static_cast<IVEngineClient*>(interfaceFactory(VENGINE_CLIENT_INTERFACE_VERSION, nullptr));
	if (!engineClient)
	{
		assert(false && "Unable to load engineClient!");
		Log(WARNING, false, "Unable to load engineClient!");
		this->noUnload = true;
		return false;
	}

	Log(INFO, true, "Loading g_pPlayerInfoManager...");
	g_pPlayerInfoManager = static_cast<IPlayerInfoManager*>(gameServerFactory(INTERFACEVERSION_PLAYERINFOMANAGER, nullptr));
	if (!g_pPlayerInfoManager)
	{
		assert(false && "Unable to load g_pPlayerInfoManager!");
		Log(WARNING, false, "Unable to load g_pPlayerInfoManager!");
		this->noUnload = true;
		return false;
	}

	Log(INFO, true, "Loading g_pGlobals...");
	g_pGlobals = g_pPlayerInfoManager->GetGlobalVars();
	if (!g_pGlobals)
	{
		assert(false && "Unable to load g_pGlobals!");
		Log(WARNING, false, "Unable to load g_pGlobals!");
		this->noUnload = true;
		return false;
	}

	// big ol' try catch because game has a TerminateProcess handler for exceptions...
	// why this wasn't here is mystifying, - 10/2024 NULLderef
	try {

		Log(INFO, true, "Executing game patches...");

		if (!Patches::EnablePatches())
		{
			this->noUnload = true;
			throw std::runtime_error("Failed to enable patches!");
		}

#if _WIN32
		// MinHook initialization and hooking.
		Log(INFO, true, "Initializing MinHook and hooking functions...");


#else // Linux Hooking. Due to the way this plugin is structured, it's currently not possible to compile this for Linux. Literally 1984 I know, but I don't have enough time or experience to figure it out by myself. One day.
#endif // _WIN32

	} catch (const std::exception& ex) {
		Log(INFO, false, "Failed to perform patch and hook operations! :( Exception: \"%s\"", ex.what());
		assert(false && "Patch and hook failure!");
		this->noUnload = true;
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

	Log(INFO, false, "Loaded plugin! Yay! :D");
	pluginLoaded = true;

	return true;
}

/**
 * @brief Called when the plugin is shutting down. Also called when the plugin fails to load.
 */
void CP2SMPlusPlusPlugin::Unload()
{
	// If the plugin errors for some reason, prevent it from unloading.
	if (noUnload)
	{
		noUnload = false;
		Log(WARNING, true, "Failed to load plugin!");
		MessageBox(GeneralGUI::GetWindowHandle(), "P2SM++ ran into a error when starting!\nPlease check the console for more info!", "P2SM++ Startup Error", MB_OK | MB_ICONERROR);
		return;
	}

	Log(INFO, false, "Unloading Plugin...");

	try
	{
#if _WIN32
		Log(INFO, true, "Un-patching game patches...");

		if (!Patches::DisablePatches())
		{
			throw std::runtime_error("Failed to disable patches!");
		}

		Log(INFO, true, "Disconnecting hooked functions and un-initializing MinHook...");
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
#else // Linux Hooking. Due to the way this plugin is structured, it's currently not possible to compile this for Linux. Literally 1984 I know, but I don't have enough time or experience to figure it out by myself. One day.
#endif
	}
	catch (const std::exception& ex)
	{
		assert(false && "Failed to fully unload!");
		Log(INFO, false, R"(Encountered error when unload plugin! :( Exception: "%s")", ex.what());
		Log(ERROR, false, "P2:MM failed to unload!\nGame has to be shutdown as possibly some other patches/hooks are still connected which can cause issues!");
	}

	// Turn every ConVar/ConCommand back to normal.
	Log(INFO, true, "Reverting changed ConVars and ConCommands...");

	// NPC's will move jankly again.
	Log(INFO, true, "cl_localnetworkbackdoor...");
	if (ConVar* lnbCVar = g_pCVar->FindVar("cl_localnetworkbackdoor"))
		lnbCVar->SetValue(1);

	// Cheats flag readded, disabled.
	Log(INFO, true, "r_drawscreenoverlay...");
	if (ConVar* screenCVar = g_pCVar->FindVar("r_drawscreenoverlay"))
	{
		screenCVar->AddFlags(FCVAR_CHEAT);
		screenCVar->SetValue(0);
	}

	// Need cheats to switch again.
	Log(INFO, true, "in_forceuser...");
	if (ConVar* ifuCVar = g_pCVar->FindVar("in_forceuser"))
		ifuCVar->AddFlags(FCVAR_CHEAT);

	Log(INFO, true, "Unregistering ConVars and ConCommands...");
	ConVar_Unregister();

	Log(INFO, true, "Disconnecting tier libraries...");
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();

	pluginLoaded = false;
	Log(INFO, false, "Plugin unloaded! Goodbye!");
}

void	CP2SMPlusPlusPlugin::Pause() {}

void	CP2SMPlusPlusPlugin::UnPause() {}

/**
 * @brief Description of plugin outputted when the "plugin_print" console command is executed.
 * @return Plugin description.
 */
const char* CP2SMPlusPlusPlugin::GetPluginDescription()
{
	return "P2SourceModPlusPlus Plugin | Plugin Version: " P2SMPLUSPLUS_PLUGIN_VERSION;
}

void CP2SMPlusPlusPlugin::LevelInit(const char* mapName) {}

void CP2SMPlusPlusPlugin::ServerActivate(Edict* edictList, int edictCount, int clientMax) {}

void CP2SMPlusPlusPlugin::GameFrame(bool simulating) {}

void CP2SMPlusPlusPlugin::LevelShutdown() {}

void CP2SMPlusPlusPlugin::ClientActive(Edict* edict) {}

/**
 * @brief Player has fully connected to the server, but the player entity isn't fully initialized yet.
 * @param edict Edict of player that has fully connected.
 */
void CP2SMPlusPlusPlugin::ClientFullyConnect(Edict* edict)
{
	// Make sure the r_drawscreenoverlay ConVar is enabled for connecting clients.
	engineServer->ClientCommand(edict, "r_drawscreenoverlay 1");
	Log(WARNING, true, "r_drawscreenoverlay enabled for client.");
}

void CP2SMPlusPlusPlugin::ClientDisconnect(Edict* edict) {}

void CP2SMPlusPlusPlugin::ClientPutInServer(Edict* edict, char const* playerName) {}

void CP2SMPlusPlusPlugin::SetCommandClient(int index) {}

void CP2SMPlusPlusPlugin::ClientSettingsChanged(Edict* edict) {}

PluginResult CP2SMPlusPlusPlugin::ClientConnect(bool* allowConnect, Edict* edict, const char* name, const char* ipAddress, char* reject, int maxRejectLen) { return PluginResult::Continue; }

PluginResult CP2SMPlusPlusPlugin::ClientCommand(Edict* edict, const CCommand& args) { return PluginResult::Continue; }

PluginResult CP2SMPlusPlusPlugin::NetworkIDValidated(const char* username, const char* networkID) { return PluginResult::Continue; }

void CP2SMPlusPlusPlugin::OnQueryCvarValueFinished(QueryCvarCookie cookie, Edict* edict, QueryCvarValueStatus status, const char* cvarName, const char* cvarValue) {}

void CP2SMPlusPlusPlugin::OnEdictAllocated(Edict* edict) {}

void CP2SMPlusPlusPlugin::OnEdictFreed(const Edict* edict) {}
