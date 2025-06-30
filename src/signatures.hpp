/*******************************************************************
* @file   signatures.hpp
* @brief  Header to store all gather memory signatures or offsets used by the plugin.
*         This is based on SAR's Offsets.hpp, but because P2SM++ is designed to only
*         support the latest version of Portal 2, it's only the one file with signatures
*         and offsets.
* @author SAR Team, Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef SIGS_HPP
#define SIGS_HPP

#define OFFSET(name, win, linux) extern int name;
#define SIGSCAN(name, win, linux) const char* name;

namespace Signatures
{
#include "signatures_portal2.hpp"
}

#undef OFFSET
#undef SIGSCAN

#endif
