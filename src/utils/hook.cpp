/*******************************************************************
* @file   hook.cpp
* @brief  Function hooking.
* @author SAR Team, Orsell
*********************************************************************/

#include "stdafx.hpp"
#include "utils/hook.hpp"

#include "utils.hpp"

template <typename T = void*>
void Hook::SetFunc(T function, const bool enable)
{
    this->func = static_cast<void*>(function);
    Memory::UnProtect(this->func, 5);
    if (enable)
        this->Enable();
}

void Hook::Enable()
{
    if (this->locked)
        return;
    if (this->enabled)
        return;
    if (!this->func || !this->hook)
        return;
    memcpy(this->origCode, this->func, sizeof this->origCode);
    auto ptr = static_cast<uint8_t*>(this->func);
    ptr[0] = 0xE9;  // JMP
    *reinterpret_cast<uint32_t*>(ptr + 1) = reinterpret_cast<uintptr_t>(this->hook) - (reinterpret_cast<uintptr_t>(ptr) + 5);
    this->enabled = true;
}

void Hook::Disable(const bool lock)
{
    if (lock)
        this->locked = true;
    if (!this->enabled)
        return;
    if (!this->func || !this->hook)
        return;
    memcpy(this->func, this->origCode, sizeof this->origCode);
    this->enabled = false;
}

void Hook::DisableAll()
{
    for (Hook* hook : Hook::GetHooks())
        hook->Disable(true);
}


