/*******************************************************************
* @file   stdafx.hpp
* @brief  Precompiled header of common includes for the plugin.
* @author Orsell
* @date   06 2025
*********************************************************************/

#pragma once

#ifndef PH_HPP
#define PH_HPP

// MARK: Windows includes
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <winerror.h>
#endif

// MARK: Common types/used includes
#include <memory>
#include <cstdlib> /* NULL, malloc/free */
#include <cstdio> /* printf, fgets */
#include <cstring> /* strcmp, strcpy_s, strtok_s */
#include <string> /* std::string */
#include <cassert> /* asserts */
#include <stdexcept>
#include <cmath> /* math */
#include <vector> /* std::vector */
#include <set> /* std::set */
#include <map> /* std::map */
#include <unordered_map> /* std::unordered_map */
#include <span>
#include <algorithm> /* iterators */
#include <iostream> /* cout, etc. */
#include <sstream>
#include <filesystem>
#include <cstdint>

#endif