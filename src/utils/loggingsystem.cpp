/*********************************************************************
* @file   loggingsystem.cpp
* @brief  Logging system for the plugin.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"   
#include "utils/loggingsystem.hpp"

#include "tier0/dbg.h"

#include "commands.hpp"

/**
 * @brief Logging for the plugin by adding a prefix and line break.
 * @param level Log level for log message. 0 = Msg/DevMsg, 1 = Warning/DevWarning, 2 = Error WILL STOP ENGINE!
 * @param dev Whether this log message should only appear when "p2sm_developer" is enabled.
 * @param message Message to log. Max character limit of 1024 characters.	
 * @param ... Message formatting.
 */
void Log(const LogLevel level, const bool dev, const char* message, ...)
{
    // Stop developer messages when p2mm_developer isn't enabled. Command line value is also checked as ConVars could not be registered yet.
    if (dev && !(p2sm_developer.GetBool() || CommandLine()->ParmValue("+p2sm_developer", "0")) && level != ERROR)
        return;

    // Take our log message and format any arguments it has into the message.
    va_list argPtr;
    char szFormattedText[1024] = { 0 };
    va_start(argPtr, message);
    vsnprintf(szFormattedText, sizeof(szFormattedText), message, argPtr);
    va_end(argPtr);

    // Add a header to the log message.
    char completeMsg[1024] = { 0 };
    snprintf(completeMsg, sizeof(completeMsg), "(P2SourceMod++ PLUGIN): %s\n", szFormattedText);

    switch (level)
    {
    case (INFO):
        ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
        return;
    case (WARNING):
        Warning(completeMsg);
        return;
    case (ERROR):
        Warning("(P2SourceMod++ PLUGIN):\n!!!ERROR ERROR ERROR!!!:\nA FATAL ERROR OCCURED WITH THE ENGINE:\n%s", completeMsg);
        Error(completeMsg);
        return;
    default:
        Warning("(P2SourceMod++ PLUGIN): Log level set outside of INFO-ERROR, \"%i\". Defaulting to level INFO.\n", level);
        ConColorMsg(P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR, completeMsg);
    }
}
