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

#define ART_MODULE_NAME     ART_CRC32

#include "ART_CRC32.h"
#include "ART_ErrorHandling.h"

UInt32  * crc32_2bit_table = 0;
UInt32  * crc32_4bit_table = 0;
UInt32  * crc32_8bit_table = 0;

ART_MODULE_INITIALISATION_FUNCTION_EXEC_ONLY_ONCE
(
    crc32_2bit_table = ALLOC_ARRAY( UInt32, 4 );

    crc32_2bit_table[0] = 0x00000000L;
    crc32_2bit_table[1] = 0x76dc4190L;
    crc32_2bit_table[2] = 0xedb88320L;
    crc32_2bit_table[3] = 0x9b64c2b0L;

    crc32_4bit_table = ALLOC_ARRAY( UInt32, 16 );

    crc32_4bit_table[ 0] = 0x00000000L;
    crc32_4bit_table[ 1] = 0x1db71064L;
    crc32_4bit_table[ 2] = 0x3b6e20c8L;
    crc32_4bit_table[ 3] = 0x26d930acL;
    crc32_4bit_table[ 4] = 0x76dc4190L;
    crc32_4bit_table[ 5] = 0x6b6b51f4L;
    crc32_4bit_table[ 6] = 0x4db26158L;
    crc32_4bit_table[ 7] = 0x5005713cL;
    crc32_4bit_table[ 8] = 0xedb88320L;
    crc32_4bit_table[ 9] = 0xf00f9344L;
    crc32_4bit_table[10] = 0xd6d6a3e8L;
    crc32_4bit_table[11] = 0xcb61b38cL;
    crc32_4bit_table[12] = 0x9b64c2b0L;
    crc32_4bit_table[13] = 0x86d3d2d4L;
    crc32_4bit_table[14] = 0xa00ae278L;
    crc32_4bit_table[15] = 0xbdbdf21cL;

    crc32_8bit_table = ALLOC_ARRAY( UInt32, 256 );

    crc32_8bit_table[  0] = 0x00000000L;
    crc32_8bit_table[  1] = 0x77073096L;
    crc32_8bit_table[  2] = 0xee0e612cL;
    crc32_8bit_table[  3] = 0x990951baL;
    crc32_8bit_table[  4] = 0x076dc419L;
    crc32_8bit_table[  5] = 0x706af48fL;
    crc32_8bit_table[  6] = 0xe963a535L;
    crc32_8bit_table[  7] = 0x9e6495a3L;
    crc32_8bit_table[  8] = 0x0edb8832L;
    crc32_8bit_table[  9] = 0x79dcb8a4L;
    crc32_8bit_table[ 10] = 0xe0d5e91eL;
    crc32_8bit_table[ 11] = 0x97d2d988L;
    crc32_8bit_table[ 12] = 0x09b64c2bL;
    crc32_8bit_table[ 13] = 0x7eb17cbdL;
    crc32_8bit_table[ 14] = 0xe7b82d07L;
    crc32_8bit_table[ 15] = 0x90bf1d91L;
    crc32_8bit_table[ 16] = 0x1db71064L;
    crc32_8bit_table[ 17] = 0x6ab020f2L;
    crc32_8bit_table[ 18] = 0xf3b97148L;
    crc32_8bit_table[ 19] = 0x84be41deL;
    crc32_8bit_table[ 20] = 0x1adad47dL;
    crc32_8bit_table[ 21] = 0x6ddde4ebL;
    crc32_8bit_table[ 22] = 0xf4d4b551L;
    crc32_8bit_table[ 23] = 0x83d385c7L;
    crc32_8bit_table[ 24] = 0x136c9856L;
    crc32_8bit_table[ 25] = 0x646ba8c0L;
    crc32_8bit_table[ 26] = 0xfd62f97aL;
    crc32_8bit_table[ 27] = 0x8a65c9ecL;
    crc32_8bit_table[ 28] = 0x14015c4fL;
    crc32_8bit_table[ 29] = 0x63066cd9L;
    crc32_8bit_table[ 30] = 0xfa0f3d63L;
    crc32_8bit_table[ 31] = 0x8d080df5L;
    crc32_8bit_table[ 32] = 0x3b6e20c8L;
    crc32_8bit_table[ 33] = 0x4c69105eL;
    crc32_8bit_table[ 34] = 0xd56041e4L;
    crc32_8bit_table[ 35] = 0xa2677172L;
    crc32_8bit_table[ 36] = 0x3c03e4d1L;
    crc32_8bit_table[ 37] = 0x4b04d447L;
    crc32_8bit_table[ 38] = 0xd20d85fdL;
    crc32_8bit_table[ 39] = 0xa50ab56bL;
    crc32_8bit_table[ 40] = 0x35b5a8faL;
    crc32_8bit_table[ 41] = 0x42b2986cL;
    crc32_8bit_table[ 42] = 0xdbbbc9d6L;
    crc32_8bit_table[ 43] = 0xacbcf940L;
    crc32_8bit_table[ 44] = 0x32d86ce3L;
    crc32_8bit_table[ 45] = 0x45df5c75L;
    crc32_8bit_table[ 46] = 0xdcd60dcfL;
    crc32_8bit_table[ 47] = 0xabd13d59L;
    crc32_8bit_table[ 48] = 0x26d930acL;
    crc32_8bit_table[ 49] = 0x51de003aL;
    crc32_8bit_table[ 50] = 0xc8d75180L;
    crc32_8bit_table[ 51] = 0xbfd06116L;
    crc32_8bit_table[ 52] = 0x21b4f4b5L;
    crc32_8bit_table[ 53] = 0x56b3c423L;
    crc32_8bit_table[ 54] = 0xcfba9599L;
    crc32_8bit_table[ 55] = 0xb8bda50fL;
    crc32_8bit_table[ 56] = 0x2802b89eL;
    crc32_8bit_table[ 57] = 0x5f058808L;
    crc32_8bit_table[ 58] = 0xc60cd9b2L;
    crc32_8bit_table[ 59] = 0xb10be924L;
    crc32_8bit_table[ 60] = 0x2f6f7c87L;
    crc32_8bit_table[ 61] = 0x58684c11L;
    crc32_8bit_table[ 62] = 0xc1611dabL;
    crc32_8bit_table[ 63] = 0xb6662d3dL;
    crc32_8bit_table[ 64] = 0x76dc4190L;
    crc32_8bit_table[ 65] = 0x01db7106L;
    crc32_8bit_table[ 66] = 0x98d220bcL;
    crc32_8bit_table[ 67] = 0xefd5102aL;
    crc32_8bit_table[ 68] = 0x71b18589L;
    crc32_8bit_table[ 69] = 0x06b6b51fL;
    crc32_8bit_table[ 70] = 0x9fbfe4a5L;
    crc32_8bit_table[ 71] = 0xe8b8d433L;
    crc32_8bit_table[ 72] = 0x7807c9a2L;
    crc32_8bit_table[ 73] = 0x0f00f934L;
    crc32_8bit_table[ 74] = 0x9609a88eL;
    crc32_8bit_table[ 75] = 0xe10e9818L;
    crc32_8bit_table[ 76] = 0x7f6a0dbbL;
    crc32_8bit_table[ 77] = 0x086d3d2dL;
    crc32_8bit_table[ 78] = 0x91646c97L;
    crc32_8bit_table[ 79] = 0xe6635c01L;
    crc32_8bit_table[ 80] = 0x6b6b51f4L;
    crc32_8bit_table[ 81] = 0x1c6c6162L;
    crc32_8bit_table[ 82] = 0x856530d8L;
    crc32_8bit_table[ 83] = 0xf262004eL;
    crc32_8bit_table[ 84] = 0x6c0695edL;
    crc32_8bit_table[ 85] = 0x1b01a57bL;
    crc32_8bit_table[ 86] = 0x8208f4c1L;
    crc32_8bit_table[ 87] = 0xf50fc457L;
    crc32_8bit_table[ 88] = 0x65b0d9c6L;
    crc32_8bit_table[ 89] = 0x12b7e950L;
    crc32_8bit_table[ 90] = 0x8bbeb8eaL;
    crc32_8bit_table[ 91] = 0xfcb9887cL;
    crc32_8bit_table[ 92] = 0x62dd1ddfL;
    crc32_8bit_table[ 93] = 0x15da2d49L;
    crc32_8bit_table[ 94] = 0x8cd37cf3L;
    crc32_8bit_table[ 95] = 0xfbd44c65L;
    crc32_8bit_table[ 96] = 0x4db26158L;
    crc32_8bit_table[ 97] = 0x3ab551ceL;
    crc32_8bit_table[ 98] = 0xa3bc0074L;
    crc32_8bit_table[ 99] = 0xd4bb30e2L;
    crc32_8bit_table[100] = 0x4adfa541L;
    crc32_8bit_table[101] = 0x3dd895d7L;
    crc32_8bit_table[102] = 0xa4d1c46dL;
    crc32_8bit_table[103] = 0xd3d6f4fbL;
    crc32_8bit_table[104] = 0x4369e96aL;
    crc32_8bit_table[105] = 0x346ed9fcL;
    crc32_8bit_table[106] = 0xad678846L;
    crc32_8bit_table[107] = 0xda60b8d0L;
    crc32_8bit_table[108] = 0x44042d73L;
    crc32_8bit_table[109] = 0x33031de5L;
    crc32_8bit_table[110] = 0xaa0a4c5fL;
    crc32_8bit_table[111] = 0xdd0d7cc9L;
    crc32_8bit_table[112] = 0x5005713cL;
    crc32_8bit_table[113] = 0x270241aaL;
    crc32_8bit_table[114] = 0xbe0b1010L;
    crc32_8bit_table[115] = 0xc90c2086L;
    crc32_8bit_table[116] = 0x5768b525L;
    crc32_8bit_table[117] = 0x206f85b3L;
    crc32_8bit_table[118] = 0xb966d409L;
    crc32_8bit_table[119] = 0xce61e49fL;
    crc32_8bit_table[120] = 0x5edef90eL;
    crc32_8bit_table[121] = 0x29d9c998L;
    crc32_8bit_table[122] = 0xb0d09822L;
    crc32_8bit_table[123] = 0xc7d7a8b4L;
    crc32_8bit_table[124] = 0x59b33d17L;
    crc32_8bit_table[125] = 0x2eb40d81L;
    crc32_8bit_table[126] = 0xb7bd5c3bL;
    crc32_8bit_table[127] = 0xc0ba6cadL;
    crc32_8bit_table[128] = 0xedb88320L;
    crc32_8bit_table[129] = 0x9abfb3b6L;
    crc32_8bit_table[130] = 0x03b6e20cL;
    crc32_8bit_table[131] = 0x74b1d29aL;
    crc32_8bit_table[132] = 0xead54739L;
    crc32_8bit_table[133] = 0x9dd277afL;
    crc32_8bit_table[134] = 0x04db2615L;
    crc32_8bit_table[135] = 0x73dc1683L;
    crc32_8bit_table[136] = 0xe3630b12L;
    crc32_8bit_table[137] = 0x94643b84L;
    crc32_8bit_table[138] = 0x0d6d6a3eL;
    crc32_8bit_table[139] = 0x7a6a5aa8L;
    crc32_8bit_table[140] = 0xe40ecf0bL;
    crc32_8bit_table[141] = 0x9309ff9dL;
    crc32_8bit_table[142] = 0x0a00ae27L;
    crc32_8bit_table[143] = 0x7d079eb1L;
    crc32_8bit_table[144] = 0xf00f9344L;
    crc32_8bit_table[145] = 0x8708a3d2L;
    crc32_8bit_table[146] = 0x1e01f268L;
    crc32_8bit_table[147] = 0x6906c2feL;
    crc32_8bit_table[148] = 0xf762575dL;
    crc32_8bit_table[149] = 0x806567cbL;
    crc32_8bit_table[150] = 0x196c3671L;
    crc32_8bit_table[151] = 0x6e6b06e7L;
    crc32_8bit_table[152] = 0xfed41b76L;
    crc32_8bit_table[153] = 0x89d32be0L;
    crc32_8bit_table[154] = 0x10da7a5aL;
    crc32_8bit_table[155] = 0x67dd4accL;
    crc32_8bit_table[156] = 0xf9b9df6fL;
    crc32_8bit_table[157] = 0x8ebeeff9L;
    crc32_8bit_table[158] = 0x17b7be43L;
    crc32_8bit_table[159] = 0x60b08ed5L;
    crc32_8bit_table[160] = 0xd6d6a3e8L;
    crc32_8bit_table[161] = 0xa1d1937eL;
    crc32_8bit_table[162] = 0x38d8c2c4L;
    crc32_8bit_table[163] = 0x4fdff252L;
    crc32_8bit_table[164] = 0xd1bb67f1L;
    crc32_8bit_table[165] = 0xa6bc5767L;
    crc32_8bit_table[166] = 0x3fb506ddL;
    crc32_8bit_table[167] = 0x48b2364bL;
    crc32_8bit_table[168] = 0xd80d2bdaL;
    crc32_8bit_table[169] = 0xaf0a1b4cL;
    crc32_8bit_table[170] = 0x36034af6L;
    crc32_8bit_table[171] = 0x41047a60L;
    crc32_8bit_table[172] = 0xdf60efc3L;
    crc32_8bit_table[173] = 0xa867df55L;
    crc32_8bit_table[174] = 0x316e8eefL;
    crc32_8bit_table[175] = 0x4669be79L;
    crc32_8bit_table[176] = 0xcb61b38cL;
    crc32_8bit_table[177] = 0xbc66831aL;
    crc32_8bit_table[178] = 0x256fd2a0L;
    crc32_8bit_table[179] = 0x5268e236L;
    crc32_8bit_table[180] = 0xcc0c7795L;
    crc32_8bit_table[181] = 0xbb0b4703L;
    crc32_8bit_table[182] = 0x220216b9L;
    crc32_8bit_table[183] = 0x5505262fL;
    crc32_8bit_table[184] = 0xc5ba3bbeL;
    crc32_8bit_table[185] = 0xb2bd0b28L;
    crc32_8bit_table[186] = 0x2bb45a92L;
    crc32_8bit_table[187] = 0x5cb36a04L;
    crc32_8bit_table[188] = 0xc2d7ffa7L;
    crc32_8bit_table[189] = 0xb5d0cf31L;
    crc32_8bit_table[190] = 0x2cd99e8bL;
    crc32_8bit_table[191] = 0x5bdeae1dL;
    crc32_8bit_table[192] = 0x9b64c2b0L;
    crc32_8bit_table[193] = 0xec63f226L;
    crc32_8bit_table[194] = 0x756aa39cL;
    crc32_8bit_table[195] = 0x026d930aL;
    crc32_8bit_table[196] = 0x9c0906a9L;
    crc32_8bit_table[197] = 0xeb0e363fL;
    crc32_8bit_table[198] = 0x72076785L;
    crc32_8bit_table[199] = 0x05005713L;
    crc32_8bit_table[200] = 0x95bf4a82L;
    crc32_8bit_table[201] = 0xe2b87a14L;
    crc32_8bit_table[202] = 0x7bb12baeL;
    crc32_8bit_table[203] = 0x0cb61b38L;
    crc32_8bit_table[204] = 0x92d28e9bL;
    crc32_8bit_table[205] = 0xe5d5be0dL;
    crc32_8bit_table[206] = 0x7cdcefb7L;
    crc32_8bit_table[207] = 0x0bdbdf21L;
    crc32_8bit_table[208] = 0x86d3d2d4L;
    crc32_8bit_table[209] = 0xf1d4e242L;
    crc32_8bit_table[210] = 0x68ddb3f8L;
    crc32_8bit_table[211] = 0x1fda836eL;
    crc32_8bit_table[212] = 0x81be16cdL;
    crc32_8bit_table[213] = 0xf6b9265bL;
    crc32_8bit_table[214] = 0x6fb077e1L;
    crc32_8bit_table[215] = 0x18b74777L;
    crc32_8bit_table[216] = 0x88085ae6L;
    crc32_8bit_table[217] = 0xff0f6a70L;
    crc32_8bit_table[218] = 0x66063bcaL;
    crc32_8bit_table[219] = 0x11010b5cL;
    crc32_8bit_table[220] = 0x8f659effL;
    crc32_8bit_table[221] = 0xf862ae69L;
    crc32_8bit_table[222] = 0x616bffd3L;
    crc32_8bit_table[223] = 0x166ccf45L;
    crc32_8bit_table[224] = 0xa00ae278L;
    crc32_8bit_table[225] = 0xd70dd2eeL;
    crc32_8bit_table[226] = 0x4e048354L;
    crc32_8bit_table[227] = 0x3903b3c2L;
    crc32_8bit_table[228] = 0xa7672661L;
    crc32_8bit_table[229] = 0xd06016f7L;
    crc32_8bit_table[230] = 0x4969474dL;
    crc32_8bit_table[231] = 0x3e6e77dbL;
    crc32_8bit_table[232] = 0xaed16a4aL;
    crc32_8bit_table[233] = 0xd9d65adcL;
    crc32_8bit_table[234] = 0x40df0b66L;
    crc32_8bit_table[235] = 0x37d83bf0L;
    crc32_8bit_table[236] = 0xa9bcae53L;
    crc32_8bit_table[237] = 0xdebb9ec5L;
    crc32_8bit_table[238] = 0x47b2cf7fL;
    crc32_8bit_table[239] = 0x30b5ffe9L;
    crc32_8bit_table[240] = 0xbdbdf21cL;
    crc32_8bit_table[241] = 0xcabac28aL;
    crc32_8bit_table[242] = 0x53b39330L;
    crc32_8bit_table[243] = 0x24b4a3a6L;
    crc32_8bit_table[244] = 0xbad03605L;
    crc32_8bit_table[245] = 0xcdd70693L;
    crc32_8bit_table[246] = 0x54de5729L;
    crc32_8bit_table[247] = 0x23d967bfL;
    crc32_8bit_table[248] = 0xb3667a2eL;
    crc32_8bit_table[249] = 0xc4614ab8L;
    crc32_8bit_table[250] = 0x5d681b02L;
    crc32_8bit_table[251] = 0x2a6f2b94L;
    crc32_8bit_table[252] = 0xb40bbe37L;
    crc32_8bit_table[253] = 0xc30c8ea1L;
    crc32_8bit_table[254] = 0x5a05df1bL;
    crc32_8bit_table[255] = 0x2d02ef8dL;
,
    (void) art_gv;
    // module has no code that gets executed on every startup
)

ART_MODULE_SHUTDOWN_FUNCTION_EXEC_ONLY_ONCE
(
    FREE_ARRAY( crc32_2bit_table );
    FREE_ARRAY( crc32_4bit_table );
    FREE_ARRAY( crc32_8bit_table );
,
    (void) art_gv;
    // module has no code that gets executed during every shutdown
)

void crc32_verify_tables    (
        ART_GV  * art_gv
        )
{
    (void) art_gv;
    
    int pattern;

    for (pattern = 0; pattern < 4; pattern++)
    {
        UInt32 crc = pattern;
        crc32_update_with_1bit(&crc, 0);
        crc32_update_with_1bit(&crc, 0);

        if (crc32_2bit_table[pattern] != crc)
            ART_ERRORHANDLING_FATAL_ERROR(
                "crc32_2bit_table[ x%02x] == 0x%08x (should be 0x%08x)"
                ,   pattern
                ,   (unsigned int) crc32_2bit_table[pattern]
                ,   (unsigned int) crc
                );
    }

    for (pattern = 0; pattern < 16; pattern++)
    {
        UInt32 crc = pattern;

        crc32_update_with_2bits(&crc, 0);
        crc32_update_with_2bits(&crc, 0);

        if (crc32_4bit_table[pattern] != crc)
            ART_ERRORHANDLING_FATAL_ERROR(
                "crc32_4bit_table[ x%02x] == 0x%08x (should be 0x%08x)"
                ,   pattern
                ,   (unsigned int) crc32_4bit_table[pattern]
                ,   (unsigned int) crc
                );
    }

    for (pattern = 0; pattern < 256; pattern++)
    {
        UInt32 crc = pattern;

        crc32_update_with_4bits(&crc, 0);
        crc32_update_with_4bits(&crc, 0);

        if (crc32_8bit_table[pattern] != crc)
            ART_ERRORHANDLING_FATAL_ERROR(
                "crc32_8bit_table[ x%02x] == 0x%08x (should be 0x%08x)"
                ,   pattern
                ,   (unsigned int) crc32_8bit_table[pattern]
                ,   (unsigned int) crc
                );
    }
}

void crc32_update_with_1bit(
        UInt32 * crc, unsigned int data
        )
{
    *crc = ((*crc ^ data) & 1) ? (*crc >> 1) ^ CRC32_POLY : (*crc >> 1);
}

void crc32_update_with_2bits(
        UInt32 * crc, unsigned int data
        )
{
    *crc = crc32_2bit_table[(*crc ^ data) & 0x3] ^ (*crc >> 2);
}

void crc32_update_with_4bits(
        UInt32 * crc, unsigned int data
        )
{
    *crc = crc32_4bit_table[(*crc ^ data) & 0xf] ^ (*crc >> 4);
}

void crc32_update_with_8bits(
        UInt32 * crc, unsigned int data
        )
{
    *crc = crc32_8bit_table[(*crc ^ data) & 0xff] ^ (*crc >> 8);
}

void crc32_update_with_16bits(
        UInt32 * crc, unsigned int data
        )
{
    crc32_update_with_8bits(crc, data);
    crc32_update_with_8bits(crc, data >> 8);
}

void crc32_update_with_32bits(
        UInt32 * crc, UInt32 data
        )
{
    crc32_update_with_16bits(crc, data);
    crc32_update_with_16bits(crc, data >> 16);
}

void crc32_update_with_64bits(
        UInt32 * crc, UInt64 data
        )
{
    crc32_update_with_32bits(crc, (UInt32) data);
    crc32_update_with_32bits(crc, (UInt32)(data >> 32));
}

void crc32_update_with_pointer(
        UInt32 * crc, void * pointer
        )
{
#ifdef _ART2_32_BIT_LONG_
    crc32_update_with_32bits(crc, (UInt32) pointer);
#endif
#ifdef _ART2_64_BIT_LONG_
    crc32_update_with_64bits(crc, (UInt64) pointer);
#endif
}

void crc32_update_with_data(
        UInt32 * crc, const void * pointer, unsigned long length
        )
{
    const Byte * byte_ptr = (const Byte *)pointer;
    const Byte * stop_ptr = byte_ptr + length;
    while (byte_ptr < stop_ptr)
        crc32_update_with_8bits(crc, *byte_ptr++);
}

void crc32_update_with_string(
        UInt32 * crc, const char * string
        )
{
    const Byte * byte_ptr = (const Byte *)string;
    char ch;
    while ((ch = *byte_ptr++) != 0)
        crc32_update_with_8bits(crc, ch);
}

void crc32_write_to_data(
        UInt32 crc, void * pointer
        )
{
    Byte * byte_ptr = (Byte *)pointer;
    *byte_ptr++ = (Byte)crc; crc >>= 8;
    *byte_ptr++ = (Byte)crc; crc >>= 8;
    *byte_ptr++ = (Byte)crc; crc >>= 8;
    *byte_ptr   = (Byte)crc;
}

/* ---------------------------------------------------------------------------
    'crc32_of_...'
        Calculate the crc32 value of various different sizes of data.
--------------------------------------------------------------------------- */
UInt32 crc32_of_uint32(
        UInt32 value
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;
    crc32_update_with_32bits(&crc, value);
    return CRC32_VALUE(crc);
}

UInt32 crc32_of_uint64(
        UInt64 value
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;
    crc32_update_with_64bits(&crc, value);
    return CRC32_VALUE(crc);
}

UInt32 crc32_of_pointer(
        const void * pointer
        )
{
#ifdef _ART2_32_BIT_LONG_
    return crc32_of_uint32((UInt32)pointer);
#endif

#ifdef _ART2_64_BIT_LONG_
    return crc32_of_uint64((UInt64)pointer);
#endif
}

UInt32 crc32_of_data(
        const void * pointer,
        unsigned long length
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;
    crc32_update_with_data(&crc, pointer, length);
    return CRC32_VALUE(crc);
}

UInt32 crc32_of_string(
        const char * string
        )
{
    UInt32 crc = CRC32_INITIAL_VALUE;
    crc32_update_with_string(&crc, string);
    return CRC32_VALUE(crc);
}

/* ======================================================================== */
