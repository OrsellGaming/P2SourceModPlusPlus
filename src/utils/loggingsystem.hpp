/*********************************************************************
* @file   loggingsystem.hpp
* @brief  Logging system for the plugin.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <color.h>

// Move this color to a LoggingSystem channel.
#define P2SMPLUSPLUS_PLUGIN_CONSOLE_COLOR  Color(100, 192, 252, 255) // Light Blue

#undef ERROR // ERROR is a Windows header defined macro that is interfering with the enum below
/**
 * @brief Log levels for all log functions.
 */
typedef enum LogLevels : std::uint8_t
{
    INFO = 0,
    WARNING,
    ERROR
} LogLevel;

/**
 * @brief The log channel struct which contains information abotu  
 */
typedef struct LogChannel
{
    const char* channelName; // Log channel logged messages belong to.
    Color messageColor;      // Color that the logged messages should print as.
    bool fileLogged;
} LogChannel;

class LoggingSystem
{
public: // MARK: LoggingSystem Public Members
#pragma region Public Members

    std::vector<LogChannel> GetLogChannels();
    bool AddLogChannel(const LogChannel channel);
    bool RemoveLogChannel(const LogChannel channel);
    
#pragma endregion

private: // MARK: LoggingSystem Private Members
#pragma region Private Members

    std::vector<LogChannel> logChannels;
    
#pragma endregion
};

void Log(LogLevel level, bool dev, const char* message, ...);

// Logging function.
//void Log(LogLevel level, bool dev, const char* pMsgFormat, ...);

#endif