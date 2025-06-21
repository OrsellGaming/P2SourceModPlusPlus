/*******************************************************************
* @file   gui.cpp
* @brief  GUI handling operations: ImGui, VGUI, and Windows Window APIs.
* @author Orsell
* @date   06 2025
*********************************************************************/

#include "stdafx.hpp"
#include "gui.hpp"

#include <cassert>

#include "utils/memory.hpp"
#include "globals.hpp"

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

    auto present = Memory::VMT<void*>(g_pDXDevice, 17);
    Log(INFO, false, "Present %p", present);
    
    return true;
}

void ImGui::Shutdown()
{
}

IDirect3DDevice9* ImGui::g_pDXDevice = nullptr;