/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */

#ifndef _ART_FOUNDATION_SYSTEM_CRC32_H_
#define _ART_FOUNDATION_SYSTEM_CRC32_H_

#include "ART_ModuleManagement.h"

ART_MODULE_INTERFACE(ART_CRC32)

#include "ART_SystemDatatypes.h"

extern UInt32  * crc32_2bit_table;
extern UInt32  * crc32_4bit_table;
extern UInt32  * crc32_8bit_table;

#define CRC32_INITIAL_VALUE             ART_UINT32_MAX

#define CRC32_POLY                      0xedb88320

#define CRC32_VALUE(_crc)               ((_crc) ^ CRC32_INITIAL_VALUE)

/* ---------------------------------------------------------------------------
    'crc32_update_with_...'
        Update a crc32 value with various different sizes of data.
--------------------------------------------------------------------------- */
void crc32_update_with_1bit(
        UInt32        * crc,
        unsigned int    data
        );

void crc32_update_with_2bits(
        UInt32        * crc,
        unsigned int    data
        );

void crc32_update_with_4bits(
        UInt32        * crc,
        unsigned int    data
        );

void crc32_update_with_8bits(
        UInt32        * crc,
        unsigned int    data
        );

void crc32_update_with_16bits(
        UInt32        * crc,
        unsigned int    data
        );

void crc32_update_with_32bits(
        UInt32  * crc,
        UInt32    data
        );

void crc32_update_with_64bits(
        UInt32  * crc,
        UInt64    data
        );

void crc32_update_with_pointer(
        UInt32  * crc,
        void    * pointer
        );

#define crc32_update_with_uint          crc32_update_with_32bits
#define crc32_update_with_int           crc32_update_with_32bits

#ifdef _ART2_32_BIT_LONG_
#define crc32_update_with_ulong         crc32_update_with_32bits
#define crc32_update_with_long          crc32_update_with_32bits
#endif

#ifdef _ART2_64_BIT_LONG_
#define crc32_update_with_ulong         crc32_update_with_64bits
#define crc32_update_with_long          crc32_update_with_64bits
#endif

void crc32_update_with_data(
        UInt32         * crc,
        const void     * pointer,
        unsigned long    length
        );

void crc32_update_with_string(
        UInt32      * crc,
        const char  * string
        );

void crc32_write_to_data(
        UInt32    crc,
        void    * pointer
        );

/* ---------------------------------------------------------------------------
    'crc32_of_...'
        Calculate the crc32 value of various different sizes of data.
--------------------------------------------------------------------------- */
UInt32 crc32_of_uint32(
        UInt32  value
        );

UInt32 crc32_of_uint64(
        UInt64  value
        );

UInt32 crc32_of_pointer(
        const void  * pointer
        );

#define crc32_of_uint                   crc32_of_uint32

#ifdef _ART2_32_BIT_LONG_
#define crc32_of_ulong                  crc32_of_uint32
#endif

#ifdef _ART2_64_BIT_LONG_
#define crc32_of_ulong                  crc32_of_uint64
#endif

UInt32 crc32_of_data(
        const void     * pointer,
        unsigned long    length
        );

UInt32 crc32_of_string(
        const char  * string
        );

#endif /* _ART_FOUNDATION_SYSTEM_CRC32_H_ */
/* ======================================================================== */
