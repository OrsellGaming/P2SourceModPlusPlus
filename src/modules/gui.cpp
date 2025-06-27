/*******************************************************************
* @file   gui.cpp
* @brief  GUI handling operations: ImGui, VGUI, and Windows Window APIs.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/gui.hpp"

#include "utils.hpp"

HWND WindowsGUI::GetWindowHandle()
{
    if (!WindowsGUI::hWnd)
        WindowsGUI::hWnd = FindWindow("Valve001", nullptr);
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "Portal 2 - Direct3D 9");
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "Portal 2 - Direct3D 9");
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "PORTAL 2");
    return WindowsGUI::hWnd;
}

bool ImGui::Init()
{
    // Get the DirectX9 device
    auto scanResult = Memory::Scan(APIDX9, "A1 ? ? ? ? 6A 00", 1);
    if (!scanResult)
        scanResult = Memory::Scan(APIVULKAN, "A1 ? ? ? ? 56 84 DB", 1);
    ImGui::g_pDXDevice = Memory::Deref<IDirect3DDevice9*>(scanResult);
    if (!ImGui::g_pDXDevice)
    {
        assert(0 && "Failed to get DirectX9 device!");
        return false;
    }

    ImGui::present = Memory::VMT<void*>(g_pDXDevice, 17);
    Log(INFO, false, "Present %p", present);
    
    return true;
}

void ImGui::Shutdown()
{
}

HWND WindowsGUI::hWnd = nullptr;
IDirect3DDevice9* ImGui::g_pDXDevice = nullptr;