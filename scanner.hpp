//===========================================================================//
//
// Author: NULLderef
// Purpose: Portal 2: Multiplayer Mod server plugin memory scanner
// 
//===========================================================================//

#ifndef SCANNER_HPP
#define SCANNER_HPP

#define SERVERDLL Memory::Modules::Get("server")
#define ENGINEDLL Memory::Modules::Get("engine")
#define CLIENTDLL Memory::Modules::Get("client")

#include <span>
#include <string>
#include <vector>
#include <memory>
#include <span>
#include <unordered_map>

namespace Memory {
	class ScannerImplementation {
	public:
		virtual uintptr_t Scan(std::span<uint8_t> region, std::string pattern, int offset) = 0;
		virtual std::vector<uintptr_t> ScanMultiple(std::span<uint8_t> region, std::string pattern, int offset) = 0;
	};

	class Scanner {
	public:
		template<typename T = void*> static T Scan(std::span<uint8_t> region, std::string pattern, int offset = 0) {
			return reinterpret_cast<T>(Scanner::Implementation().get()->Scan(region, pattern, offset));
		}

		static std::vector<uintptr_t> ScanMultiple(std::span<uint8_t> region, std::string pattern, int offset = 0) {
			return Scanner::Implementation().get()->ScanMultiple(region, pattern, offset);
		}

	private:
		static std::unique_ptr<ScannerImplementation>& Implementation();
	};
	class Modules {
	public:
		static std::span<uint8_t> Get(std::string name);

	private:
		static void PopulateModules();
		static std::unordered_map<std::string, std::span<uint8_t>> loadedModules;
	};

	void ReplacePattern(std::string target_module, std::string patternBytes, std::string replace_with);

	
	template<typename T = void*> T Rel32(void* relPtr) {
		auto rel = reinterpret_cast<uintptr_t>(relPtr);
		return reinterpret_cast<T>(rel + *reinterpret_cast<int32_t*>(rel) + sizeof(int32_t));
	}
};

#endif // SCANNER_HPP
