/*******************************************************************
* @file   memory.cpp
* @brief  Memory signature scanning and patching.
* @author SAR Team
*********************************************************************/

#include "stdafx.hpp"
#include "utils/memory.hpp"

#ifdef _WIN32
// clang-format off
#	include <windows.h>
#	include <tchar.h>
#	include <psapi.h>
// clang-format on
#else
#	include <dlfcn.h>
#	include <link.h>
#	include <sys/uio.h>
#	include <unistd.h>
#	include <signal.h>
#endif

#include "utils/loggingsystem.hpp"

#define INRANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define getBits(x) (INRANGE(((x) & (~0x20)), 'A', 'F') ? (((x) & (~0x20)) - 'A' + 0xA) : (INRANGE(x, '0', '9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

std::vector<Memory::ModuleInfo> Memory::moduleList;

bool Memory::TryGetModule(const char* moduleName, Memory::ModuleInfo* info)
{
	if (Memory::moduleList.empty())
	{
#ifdef _WIN32
		HMODULE hMods[1024];
		HANDLE pHandle = GetCurrentProcess();
		DWORD cbNeeded;
		if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded))
		{
			for (unsigned i = 0; i < (cbNeeded / sizeof(HMODULE)); ++i)
			{
				char buffer[MAX_PATH];
				if (!GetModuleFileName(hMods[i], buffer, sizeof(buffer)))
					continue;

				auto modInfo = MODULEINFO();
				if (!GetModuleInformation(pHandle, hMods[i], &modInfo, sizeof(modInfo)))
					continue;

				auto module = ModuleInfo();

				auto temp = std::string(buffer);
				auto index = temp.find_last_of("\\/");
				temp = temp.substr(index + 1, temp.length() - index);

				std::snprintf(module.name, sizeof(module.name), "%s", temp.c_str());
				module.base = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
				module.size = static_cast<uintptr_t>(modInfo.SizeOfImage);
				std::snprintf(module.path, sizeof(module.path), "%s", buffer);

				Memory::moduleList.push_back(module);
			}
		}

#else
		dl_iterate_phdr([](struct dl_phdr_info *info, size_t, void *)
		{
			auto temp = std::string(info->dlpi_name);
			auto index = temp.find_last_of("\\/");
			temp = temp.substr(index + 1, temp.length() - index);

			for (int i = 0; i < info->dlpi_phnum; ++i)
			{
				// FIXME: we really want data segments too! but +x is more important
				if (info->dlpi_phdr[i].p_flags & 1) // execute
				{
					Memory::ModuleInfo module;
					module.base = info->dlpi_addr + info->dlpi_phdr[i].p_vaddr;
					module.size = info->dlpi_phdr[i].p_memsz;
					std::strncpy(module.name, temp.c_str(), sizeof(module.name));
					std::strncpy(module.path, info->dlpi_name, sizeof(module.path));
					Memory::moduleList.push_back(module);
					break;
				}
			}

			return 0;
		}, nullptr);
#endif
	}

	for (const Memory::ModuleInfo& item : Memory::moduleList)
	{
		if (!std::strcmp(item.name, moduleName))
		{
			if (info)
				*info = item;

			return true;
		}
	}

	return false;
}
void* Memory::GetModuleHandleByName(const char* moduleName)
{
	auto info = Memory::ModuleInfo();
#ifdef _WIN32
	return (Memory::TryGetModule(moduleName, &info)) ? GetModuleHandleA(info.path) : nullptr;
#else
	return (TryGetModule(moduleName, &info)) ? dlopen(info.path, RTLD_NOLOAD | RTLD_NOW) : nullptr;
#endif
}
void Memory::CloseModuleHandle(void* moduleHandle)
{
#ifndef _WIN32
	dlclose(moduleHandle);
#endif
}

uintptr_t Memory::FindAddress(const uintptr_t start, const uintptr_t end, const char* target)
{
	const char* pattern = target;
	uintptr_t result = 0;

	for (auto position = start; position < end; ++position) {
		if (!*pattern)
			return result;

		if (*pattern == '?' || *reinterpret_cast<uint8_t*>(position) == getByte(pattern))
		{
			if (!result)
				result = position;

			if (!pattern[1] || !pattern[2])
				return result;

			pattern += (*pattern == '\?') ? 2 : 3;
		}
		else
		{
			pattern = target;
			result = 0;
		}
	}
	return 0;
}
uintptr_t Memory::Scan(const char *moduleName, const char* pattern, const int offset)
{
	uintptr_t result = 0;
	if (strlen(pattern) == 0)
	{
		// blank pattern = skip
		return result;
	}

	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info))
	{
		const auto start = info.base;
		const auto end = start + info.size;
		result = Memory::FindAddress(start, end, pattern);
		if (result)
			result += offset;
	}

	if (result == 0)
	{
		Log(WARNING, false, "Failed to find pattern \"%s\" in module \"%s\"\n", pattern, moduleName);
#ifdef _WIN32
		if (IsDebuggerPresent())
			DebugBreak();
#else
#ifdef P2SM_DEV_BUILD
		raise(SIGTRAP);
#endif
#endif
	}

	return result;
}
std::vector<uintptr_t> Memory::MultiScan(const char* moduleName, const char* pattern, const int offset)
{
	std::vector<uintptr_t> result;
	const auto length = std::strlen(pattern);

	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info))
	{
		auto start = info.base;
		const auto end = start + info.size;
		while (true) {
			if (auto addr = Memory::FindAddress(start, end, pattern))
			{
				result.push_back(addr + offset);
				start = addr + length;
			}
			else
				break;
		}
	}
	return result;
}

std::vector<uintptr_t> Memory::Scan(const char* moduleName, const Memory::Pattern* pattern)
{
	std::vector<uintptr_t> result;

	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info))
	{
		const auto start = info.base;
		const auto end = start + info.size;
		if (const auto addr = Memory::FindAddress(start, end, pattern->signature))
		{
			for (auto const& offset : pattern->offsets)
				result.push_back(addr + offset);
		}
	}
	return result;
}
std::vector<std::vector<uintptr_t>> Memory::MultiScan(const char* moduleName, const Memory::Patterns* patterns)
{
	auto results = std::vector<std::vector<uintptr_t>>();

	auto info = Memory::ModuleInfo();
	if (Memory::TryGetModule(moduleName, &info))
	{
		const auto moduleStart = info.base;
		const auto moduleEnd = moduleStart + info.size;

		for (const auto& pattern : *patterns)
		{
			const auto length = std::strlen(pattern->signature);
			auto start = moduleStart;

			while (true)
			{
				if (const auto addr = Memory::FindAddress(start, moduleEnd, pattern->signature))
				{
					auto result = std::vector<uintptr_t>();
					for (const auto& offset : pattern->offsets)
						result.push_back(addr + offset);

					results.push_back(result);
					start = addr + length;
				}
				else
					break;
			}
		}
	}
	return results;
}

Memory::Patch::~Patch()
{
	if (this->original)
	{
		this->Restore();
		delete[] this->original;
		this->original = nullptr;
	}
	if (this->patch)
	{
		delete[] this->patch;
		this->patch = nullptr;
	}
	this->isPatched = false;
}

bool Memory::Patch::Execute()
{
	if (this->isPatched)
		return true; // already executed
	unsigned char* tmpPatch = new unsigned char[this->size];
	//	We create another patch, because this->patch is gonna be deleted
	memcpy(tmpPatch, this->patch, this->size);
	auto ret = this->Execute(this->location, tmpPatch, this->size);
	delete[] tmpPatch;
	return ret;
}

bool Memory::Patch::Execute(const uintptr_t location_, unsigned char* bytes, const size_t size_)
{
	if (this->isPatched)
		return true; // already executed
	this->location = location_;
	this->size = size_;

	if (this->original)
	{
		delete[] this->original;
		this->original = nullptr;
	}
	this->original = new unsigned char[this->size];

	if (!bytes)
		return false;

	if (this->patch)
	{
		delete[] this->patch;
		this->patch = nullptr;
	}
	this->patch = new unsigned char[this->size];
	memcpy(this->patch, bytes, size_);

#ifdef _WIN32
	for (size_t i = 0; i < this->size; ++i)
	{
		if (!ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(this->location + i), &this->original[i], 1, 0))
			return false;
	}

	for (size_t i = 0; i < this->size; ++i)
	{
		if (!WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(this->location + i), &bytes[i], 1, 0))
			return false;
	}
#else
	Memory::UnProtect(reinterpret_cast<void *>(this->location), this->size);
	for (size_t i = 0; i < this->size; ++i) {
		this->original[i] = *(uint8_t *)(this->location + i);
		*(uint8_t *)(this->location + i) = bytes[i];
	}
#endif
	this->isPatched = true;
	return true;
}
bool Memory::Patch::Restore()
{
	if (!this->location || !this->original)
		return false;

	if (!this->isPatched)
		return true; // already restored
#ifdef _WIN32
	if (!WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(this->location), this->original, this->size, 0))
		return false;
#else
	//	Should be already unprotected, but just in case
	Memory::UnProtect(reinterpret_cast<void *>(this->location), this->size);
	for (size_t i = 0; i < this->size; ++i)
		{
		*(uint8_t*)(this->location + i) = this->original[i];
	}
#endif
	this->isPatched = false;
	return true;
}

bool Memory::Patch::IsPatched() const
{
	return this->isPatched;
}

bool Memory::Patch::IsInit() const
{
	return this->original != nullptr && this->patch != nullptr;
}
