/*******************************************************************
* @file   signatures.cpp
* @brief  Header to store all gather memory signatures or offsets used by the plugin.
*         This is based on SAR's Offsets.hpp, but because P2SM++ is designed to only
*         support the latest version of Portal 2, it's only the one file with signatures
*         and offsets.
* @author SAR Team, Orsell
* @date   06 2025
*********************************************************************/

#ifdef _WIN32
#define OFFSET(name, win, linux) int name = win;
#define SIGSCAN(name, win, linux) const char* name = win;
#else // !_WIN32
#warning "Linux signatures are not yet implemented"
#define OFFSET(name, win, linux) int name = linux;
#define SIGSCAN(name, win, linux) const char* name = linux;
#endif

namespace Signatures
{
#include "signatures_portal2.hpp"
}

