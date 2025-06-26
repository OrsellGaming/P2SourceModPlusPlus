/*******************************************************************
* @file   utils.h
* @brief  Util functions.
* @author Orsell, Nanoman2525, Nullderef
* @date   06 2025
*********************************************************************/

#ifndef UTILS_HPP
#define UTILS_HPP

class Vector;
class CBaseEntity;
class CBasePlayer;
struct edict_t;
typedef struct HSCRIPT__* HSCRIPT;

// Utils::HudMessage message parameters struct. Based on the one from Valve's util.h.
// See Valve Developer Community for game_text to see which field does what:
// https://developer.valvesoftware.com/wiki/Game_text
typedef struct HudMessageParams
{
    float		x = -1.f, y = 0.6f;
    int			effect = 0;
    byte		r1 = 100, g1 = 100, b1 = 100, a1 = 255;
    byte		r2 = 240, g2 = 110, b2 =   0, a2 = 255;
    float		fadeinTime = 1.5f, fadeoutTime = 0.5f, holdTime = 1.2f;
    float		fxTime = 0.25f;
    int			channel = 1;
} HudMessageParams;

// ClientPrint msgDest enum.
enum MessageDestination : std::uint8_t
{
    HUD_PRINTNOTIFY	= 1, // Works same as HUD_PRINTCONSOLE
    HUD_PRINTCONSOLE,    // Print to console.
    HUD_PRINTTALK,		 // Print to chat.
    HUD_PRINTCENTER		 // Print to top center of screen.
};

namespace Utils
{
    int			 UserIDToPlayerIndex(int userid);
    const char*	 GetPlayerName(int playerIndex);
    int			 GetSteamID(int playerIndex);
    int			 GetConVarInt(const char* cvName);
    const char*	 GetConVarString(const char* cvName);
    void		 SetConVarInt(const char* cvName, int newValue);
    void		 SetConVarString(const char* cvName, const char* newValue);
    bool		 IsBot(int playerIndex);
    int			 GetBotCount();
    int			 CurPlayerCount();
    HSCRIPT		 EntIndexScriptHandle(int entityIndex);
    CBasePlayer* PlayerByIndex(int playerEntityIndex);
    void         ClientPrint(CBasePlayer* player, MessageDestination msgDest, const char* msg, const char* param1 = nullptr, const char* param2 = nullptr, const char* param3 = nullptr, const char* param4 = nullptr);
    void         HudMessage(CBasePlayer* pPlayer, const char* pMessage, const HudMessageParams& textParms = HudMessageParams());
    CBasePlayer* GetCommandClient();
    int          GetCommandClientIndex();
    void         SetOrigin(CBaseEntity* entity, const Vector& vecOrigin, bool fireTriggers);
	int			 EdictIndex(const edict_t* pEdict);
	edict_t*	 IndexToEdict(const int entityIndex);
	inline const char* GetGameMainDir();
	const char*	 GetGameRootDir();
	inline bool  IsGameActive();
	inline bool  IsGameShutdown();
	inline int	 EntityIndex(CBaseEntity* pEntity);
}

/**
 * @brief Check if strings match,
 * @param str1 String 1.
 * @param str2 String 2.
 * @return Returns true if strings match.
 */
inline bool FStrEq(const char* str1, const char* str2)
{
	return (stricmp(str1, str2) == 0);
}

/**
 * @brief Check if a string contains a substring.
 * @param str String to check.
 * @param search Substring to find in string,
 * @return If substring is in string,
 */
inline bool FSubStr(const char* str, const char* search)
{
	return (std::strstr(str, search));
}

#endif