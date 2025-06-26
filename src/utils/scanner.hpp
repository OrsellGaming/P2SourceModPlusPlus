//===========================================================================//
//
// Author: NULLderef
// Purpose: Portal 2: Multiplayer Mod server plugin memory scanner
// 
//===========================================================================//

#pragma once

#ifndef SCANNER_HPP
#define SCANNER_HPP

namespace Memory {
	class ScannerImplementation
	{
	public:
		virtual uintptr_t Scan(std::span<uint8_t> region, std::string pattern, int offset) = 0;
		virtual std::vector<uintptr_t> ScanMultiple(std::span<uint8_t> region, std::string pattern, int offset) = 0;
	};

	class Scanner
	{
	public:
		template<typename T = void*> static T Scan(std::span<uint8_t> region, std::string pattern, int offset = 0)
		{
			return reinterpret_cast<T>(Scanner::Implementation()->Scan(region, std::move(pattern), offset));
		}

		static std::vector<uintptr_t> ScanMultiple(std::span<uint8_t> region, const std::string& pattern, int offset = 0)
		{
			return Scanner::Implementation()->ScanMultiple(region, pattern, offset);
		}

	private:
		static std::unique_ptr<ScannerImplementation>& Implementation();
	};
	class Modules
	{
	public:
		static std::span<uint8_t> Get(const std::string& name);

	private:
		static void PopulateModules();
		static std::unordered_map<std::string, std::span<uint8_t>> loadedModules;
	};

	void ReplacePattern(const std::string& targetModule, const std::string& patternBytes, const std::string& replaceWith);
	
	template<typename T = void*> T Rel32(void* relPtr)
	{
		auto rel = reinterpret_cast<uintptr_t>(relPtr);
		return reinterpret_cast<T>(rel + *reinterpret_cast<int32_t*>(rel) + sizeof(int32_t));
	}
};

#endif // SCANNER_HPP
