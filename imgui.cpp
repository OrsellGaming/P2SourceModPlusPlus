#include "imgui.h"
#include "memory.hpp"
#include "globals.hpp"

bool ImGUI::Init()
{
    // Get the DirectX9 device.
    ImGUI::g_pDevice = Memory::Deref<IDirect3DDevice9*>(Memory::Scan(DIRECTX9DLL, "A1 ? ? ? ? 6A 00", 1));
    if (!ImGUI::g_pDevice)
    {
        assert(0 && "Failed to get DirectX9 device!");
        return false;
    }

    auto present = Memory::VMT<void*>(g_pDevice, 17);
    Log(INFO, false, "Present %p", present);
    
    return true;
}

void ImGUI::Shutdown()
{
}

IDirect3DDevice9* ImGUI::g_pDevice = nullptr;