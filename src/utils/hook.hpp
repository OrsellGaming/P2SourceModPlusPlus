/*******************************************************************
* @file   memory.cpp
* @brief  Function hooking.
* @author SAR Team
*********************************************************************/

#pragma once

#ifndef HOOK_HPP
#define HOOK_HPP

#include "memory.hpp"

#ifdef _WIN32
#include "minhook/include/MinHook.h"
#endif

#define DECL_HOOK(func, name) \
	Hook h_##name(&func, #name)

class Hook
{
public:
	template <typename T = void*>
	explicit Hook(T hook, std::string name = "")
		: func(nullptr)
		, hook(static_cast<void*>(hook))
		, enabled(false)
		, hookName(std::move(name))
	{
		Hook::GetHooks().push_back(this);
	}

	~Hook() = default;

	template <typename T = void *>
	void SetFunc(T function, const bool enable = true)
	{
		this->func = static_cast<void*>(function);
		Memory::UnProtect(this->func, 5);
		if (enable)
			this->Enable();
	}

	void Enable()
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

	void Disable(const bool lock = false)
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

	static void DisableAll()
	{
		for (Hook* hook : Hook::GetHooks())
			hook->Disable(true);
	}

	static std::vector<Hook*>& GetHooks()
	{
		static std::vector<Hook*> hooks;
		return hooks;
	}

	static Hook* GetHook(const std::string& name)
	{
		for (Hook* hook : Hook::GetHooks())
			if (hook->hookName == name)
				return hook;
		
		return nullptr;
	}

	std::string GetName() const
	{
		return this->hookName;
	}

private:
	void* func;
	void* hook;
	bool enabled;
	bool locked;
	std::uint8_t origCode[5];
	std::string hookName;
};

#endif
