
#pragma once

#ifndef IMGUI_HPP
#define IMGUI_HPP

#ifdef _WIN32
#include <d3d9.h>
#else
#error "Some nerd implement"
#endif


class ImGUI
{
public:
    static bool Init();
    static void Shutdown();
private:
#ifdef _WIN32
    static IDirect3DDevice9* g_pDevice;
#else
#endif
    
};

#endif