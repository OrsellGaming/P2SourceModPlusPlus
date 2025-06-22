/*********************************************************************
* @file   gui.hpp
* @brief  GUI related functionality. Windows Window APIs, ImGUI, VGUI.  
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef GUI_HPP
#define GUI_HPP

#ifdef _WIN32
#include <d3d9.h>
#else
#error "Linux needs to implemented!"
#endif

/**
 * @brief Windows GUI functions and variables.
 */
class WindowsGUI
{
public: // MARK: WindowsGUI Public Members
#pragma region Public Members
    static HWND GetWindowHandle();
#pragma endregion

private: // MARK: WindowsGUI Private Members
#pragma region Private Members
    static HWND hWnd;
#pragma endregion
};

/**
 * @brief Valve GUI functions and variables.
 */
class VGUI
{
public: // MARK: VGUI Public Members
#pragma region Public Members
#pragma endregion

private: // MARK: VGUI Private Members
#pragma region Private Members
#pragma endregion
};

/**
 * @brief Dear ImGUI functions and variables.
 */
class ImGui
{
public: // MARK: ImGui Public Members
#pragma region Public Members 
    
    static bool Init();
    static void Shutdown();

#pragma endregion
    
private: // MARK: ImGui Private Members
#pragma region Private Members
    
#ifdef _WIN32
    static IDirect3DDevice9* g_pDXDevice;
#else
    // TODO: Implement SDL and Vulkan devices
#endif

    static void* present;

#pragma endregion
};

#endif