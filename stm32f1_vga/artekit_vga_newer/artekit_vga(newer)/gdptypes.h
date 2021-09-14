/***************************************************************************
 * STM32 VGA demo
 * Copyright (C) 2012 Artekit Italy
 * http://www.artekit.eu
 * Written by Ruben H. Meleca
 
### gditypes.h
 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

***************************************************************************/
#ifndef __GDPTYPES_H
#define __GDPTYPES_H

#define NULL 0x0

#include "stdint.h"
// Types

typedef uint32_t   u32;
typedef int32_t    i32;
typedef uint16_t   u16;
typedef int16_t    i16;
typedef uint8_t    u8;
typedef int8_t     i8;

typedef uint32_t   *pu32;
typedef int32_t    *pi32;
typedef uint16_t   *pu16;
typedef int16_t    *pi16;
typedef uint8_t    *pu8;
typedef int8_t     *pi8;

#endif // __GDPTYPES_H
