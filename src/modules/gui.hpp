
/*******************************************************************
* @file   gui.h
* @brief  GUI handling operations: ImGui, VGUI, and Windows Window APIs.
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

class GUI
{
public: // MARK: GUI Public Members
#pragma region Public Members
#pragma endregion

private: // MARK: GUI Private Members
#pragma region Private Members
    static HWND g_hWnd;
#pragma endregion
};

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
    // Implement SDL and Vulkan 
#endif

    static void* present;

#pragma endregion
};

#endif