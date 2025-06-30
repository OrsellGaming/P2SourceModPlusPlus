/*******************************************************************
* @file   gui.cpp
* @brief  GUI handling operations: ImGui, VGUI, and Windows Window APIs.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "modules/gui.hpp"

#include "utils.hpp"

bool GeneralGUI::InitializeGUISystems()
{
    Log(INFO, true, "Grabbing game window handle...");
    if (!GeneralGUI::GetWindowHandle())
    {
        //assert(0 && "Failed to get window handle!");
        Log(WARNING, false, "Failed to get window handle!");
        return false;
    }

    // Log(INFO, true, "Starting ImGUI...");
    // if (!ImGui::Init())
    // {
    //     //assert(false && "Failed to initialize ImGui!");
    //     Log(INFO, false, "Failed to initialize ImGui!");
    //     return false;
    // }

    return true;
}

/**
 * @brief Get the handle to the game Window.
 * @return Windows Window handle.
 */
HWND GeneralGUI::GetWindowHandle()
{
    if (!GeneralGUI::hWnd)
    {
        GeneralGUI::hWnd = FindWindow("Valve001", nullptr);
    }
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "Portal 2 - Direct3D 9");
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "PORTAL 2 - Vulkan");
    // if (!WindowsGUI::hWnd)
    //     WindowsGUI::hWnd = FindWindowA(nullptr, "PORTAL 2");
    return GeneralGUI::hWnd;
}

/**
 * @brief ImGui initialization function.
 * @return Returns true if ImGui initialized successfully.
 */
bool ImGui::Init()
{
    // Get the DirectX9 Device.
    // Valve used DXVK for Vulkan, so the API it uses layers over DirectX's stuff so the device handle can be used for both cases.

    IDirect3DDevice9** deviceHandlePtr;
    if (Memory::GetModuleHandleByName(MODULE_SHADERAPIDX9))
        deviceHandlePtr = Memory::Scan<IDirect3DDevice9**>(MODULE_SHADERAPIDX9, "A1 ? ? ? ? 6A 00", 1);
    else
        deviceHandlePtr = Memory::Scan<IDirect3DDevice9**>(MODULE_APIVULKAN, "A1 ? ? ? ? 56 84 DB", 1);

    if (!deviceHandlePtr)
    {
        assert(0 && "Failed to get address pointer to the DirectX9 device!");
        Log(WARNING, false, "Failed to get address pointer to the DirectX9 device!");
        return false;
    }

    // Get the device itself from the pointer pointer of the device address.
    ImGui::dXDevice = *deviceHandlePtr;
    if (!ImGui::dXDevice)
    {
        assert(0 && "Failed to get DirectX9 device!");
        Log(WARNING, false, "Failed to get DirectX9 device!");
        return false;
    }

    // Get the "Reset" class function
    ImGui::Reset = Memory::VMT<void*>(dXDevice, 16);
    Log(INFO, false, "Reset %p", Reset);

    // Get the "Present" class function used for rendering to it can be hooked.
    ImGui::Present = Memory::VMT<void*>(dXDevice, 17);
    Log(INFO, false, "Present %p", Present);


    // TODO: Add ImGui initialization calls here.

    return true;
}

void ImGui::Shutdown()
{
    // TODO: Add ImGui shutdown calls here.
}

HWND GeneralGUI::hWnd = nullptr;
IDirect3DDevice9* ImGui::dXDevice = nullptr;
void* ImGui::Reset = nullptr;
void* ImGui::Present = nullptr;
