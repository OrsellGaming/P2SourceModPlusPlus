/*******************************************************************
* @file   hook.hpp
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

#define DEFINE_HOOK(class, name) \
	Hook* class::h_##name = nullptr

#define DECL_HOOK(name) \
	static Hook* h_##name

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
	void SetFunc(T function, const bool enable = true);

	void Enable();

	void Disable(const bool lock = false);

	static void DisableAll();

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
