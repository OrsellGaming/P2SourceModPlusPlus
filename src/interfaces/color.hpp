/*******************************************************************
* @file   color.hpp
* @brief  Source Engine color interface.
* @author Orsell, Nullderef
* @date   07 2025
*********************************************************************/

#pragma once

#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

struct Color
{
    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
    uint8_t r, g, b, a;
};

#endif