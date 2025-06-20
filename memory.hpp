#pragma once
#ifdef _WIN32
// clang-format off
#	include <windows.h>
#	include <memoryapi.h>
// clang-format on
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#	define MAX_PATH 4096
#endif

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

#define SERVERDLL "server.dll"
#define ENGINEDLL "engine.dll"
#define CLIENTDLL "client.dll"
#define DIRECTX9DLL "shaderapidx9.dll"

namespace Memory
{
	struct ModuleInfo
	{
		char name[MAX_PATH];
		uintptr_t base;
		uintptr_t size;
		char path[MAX_PATH];
	};

	extern std::vector<ModuleInfo> moduleList;

	bool TryGetModule(const char* moduleName, ModuleInfo *info);
	void* GetModuleHandleByName(const char* moduleName);
	void CloseModuleHandle(void* moduleHandle);

	uintptr_t FindAddress(const uintptr_t start, const uintptr_t end, const char* target);
	uintptr_t Scan(const char* moduleName, const char* pattern, int offset = 0);
	std::vector<uintptr_t> MultiScan(const char* moduleName, const char* pattern, int offset = 0);

	class Patch
	{
	private:
		uintptr_t location;
		unsigned char* original;
		unsigned char* patch;
		size_t size;
		bool isPatched;

	public:
		~Patch();
		bool Execute();
		bool Execute(uintptr_t location, unsigned char* bytes, size_t size);
		template <size_t size>
		bool Execute(uintptr_t location, unsigned char (&bytes)[size])
		{
			return Execute(location, bytes, size);
		}
		bool Restore();
		bool IsPatched();
		bool IsInit();
	};

	struct Pattern
	{
		const char* signature;
		std::vector<int> offsets;
	};

	typedef std::vector<int> Offset;
	typedef std::vector<const Pattern *> Patterns;

#define PATTERN(name, sig, ...) \
	Memory::Pattern name { sig, Memory::Offset({__VA_ARGS__}) }
#define PATTERNS(name, ...) Memory::Patterns name({__VA_ARGS__})

	std::vector<uintptr_t> Scan(const char *moduleName, const Pattern *pattern);
	std::vector<std::vector<uintptr_t>> MultiScan(const char *moduleName, const Patterns* patterns);

	template <typename T = uintptr_t>
	T Absolute(const char *moduleName, int relative)
	{
		auto info = Memory::ModuleInfo();
		return (Memory::TryGetModule(moduleName, &info)) ? static_cast<T>(info.base + relative) : static_cast<T>(0);
	}
	template <typename T = void*>
	T GetSymbolAddress(void *moduleHandle, const char *symbolName)
	{
#ifdef _WIN32
		return static_cast<T>(GetProcAddress((HMODULE)moduleHandle, symbolName));
#else
		return (T)dlsym(moduleHandle, symbolName);
#endif
	}
	template <typename T = void*>
	inline T VMT(void* ptr, const int index)
	{
		return reinterpret_cast<T>((*static_cast<void***>(ptr))[index]);
	}
	template <typename T = uintptr_t>
	inline T Read(uintptr_t source) {
		auto rel = *reinterpret_cast<int *>(source);
		return static_cast<T>(source + rel + sizeof(rel));
	}
	template <typename T = uintptr_t>
	void Read(uintptr_t source, T* destination)
	{
		*destination = Memory::Read<T>(source);
	}
	template <typename T = void*>
	inline T Deref(uintptr_t source)
	{
		return *reinterpret_cast<T*>(source);
	}
	template <typename T = void*>
	void Deref(uintptr_t source, T* destination)
{
		*destination = Memory::Deref<T>(source);
	}
	template <typename T = void *>
	inline T DerefDeref(uintptr_t source)
	{
		return Memory::Deref<T>(Memory::Deref<uintptr_t>(source));
	}
	template <typename T = void *>
	void DerefDeref(uintptr_t source, T *destination)
	{
		*destination = Memory::DerefDeref<T>(source);
	}
	template <typename T = uintptr_t>
	T Scan(const char* moduleName, const char* pattern, const int offset = 0)
	{
		uintptr_t result = 0;

		auto info = Memory::ModuleInfo();
		if (Memory::TryGetModule(moduleName, &info))
		{
			const auto start = uintptr_t(info.base);
			const auto end = start + info.size;
			result = Memory::FindAddress(start, end, pattern);
			if (result)
				result += offset;
			if (!result)
			{
				assert(false && "Failed to find signature!");
				throw std::runtime_error("Unable to find signature!");
			}
		}
		return reinterpret_cast<T>(result);
	}

	inline void UnProtect(void *addr, size_t len)
	{
		uintptr_t startPage = reinterpret_cast<uintptr_t>(addr) & 0xFFFFF000;
		uintptr_t endPage = (reinterpret_cast<uintptr_t>(addr) + len) & 0xFFFFF000;
		uintptr_t pageLen = endPage - startPage + 0x1000;
#ifdef _WIN32
		DWORD wtf_microsoft_why_cant_this_be_null;
		VirtualProtect(reinterpret_cast<void*>(startPage), pageLen, PAGE_EXECUTE_READWRITE, &wtf_microsoft_why_cant_this_be_null);
#else
		mprotect((void *)startPage, pageLen, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif
	}
}  // namespace Memory
