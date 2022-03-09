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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSIONMACROS_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSIONMACROS_H_

/* ---------------------------------------------------------------------------
    'COLOUR_..._OF_...'
        Constants for conversion between colours.
--------------------------------------------------------------------------- */
#define INTCOL_I8_OF_I16     (1.0/255.0)
#define INTCOL_I8_OF_I32     (1.0/65535.0)
#define INTCOL_I16_OF_I32    (1.0/255.0)
#define INTCOL_I8_OF_DBL     ((1.0 - MATH_TINY_DOUBLE) * 256.0)
#define INTCOL_I16_OF_DBL    ((1.0 - MATH_TINY_DOUBLE) * 65536.0)
#define INTCOL_I32_OF_DBL    ((1.0 - MATH_TINY_DOUBLE) * 4294967296.0)
#define INTCOL_I16_OF_I8     257
#define INTCOL_I32_OF_I8     16843009
#define INTCOL_I32_OF_I16    65537
#define INTCOL_DBL_OF_I8     (1.0/255.0)
#define INTCOL_DBL_OF_I16    (1.0/65535.0)
#define INTCOL_DBL_OF_I32    (1.0/4294967295.0)

/* ---------------------------------------------------------------------------
        Constructor-like macros for those colour conversions which can
        be handled by simple multiplications and conversions of channels.

        They are grouped by the colour type they convert *from*, since this
        is the order in which they are called from ArColourConversion.c, and
        also since this improves the readability of the whole thing.
------------------------------------------------------------------------aw- */


// =====   ArGrey   ==========================================================


#define ARGREYALPHA_OF_ARGREY(_c)           ARGREYALPHA( \
                                            ARGREY_G(_c),\
                                            1.0 \
                                            )

#define ARGREY8_OF_ARGREY(_c)               ARGREY8( \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARGREY(_c)              ARGREY16( \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARGREY(_c)         ARGREYALPHA16( \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARGREY(_c)         ARGREYALPHA32( \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARGREY(_c)                 ARRGB( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c) \
                                            )

#define ARFLOATRGB_OF_ARGREY(_c)            ARFLOATRGB( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c) \
                                            )

#define ARRGBA_OF_ARGREY(_c)                ARRGBA( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARGREY(_c)           ARFLOATRGBA( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARGREY(_c)               ARRGB24( \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARGREY(_c)              ARRGBA32( \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARGREY(_c)               ARRGB48( \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARGREY(_c)              ARRGBA64( \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARGREY(_c)               ARRGB96( \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARGREY(_c)             ARRGBA128( \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREY_G(_c) * INTCOL_I32_OF_DBL, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArGreyAlpha   =====================================================


#define ARGREY_OF_ARGREYALPHA(_c)           ARGREY( \
                                            ARGREYALPHA_G(_c) \
                                            )

#define ARGREY8_OF_ARGREYALPHA(_c)          ARGREY8( \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARGREYALPHA(_c)         ARGREY16( \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARGREYALPHA(_c)    ARGREYALPHA16( \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREYALPHA32_OF_ARGREYALPHA(_c)    ARGREYALPHA32( \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB_OF_ARGREYALPHA(_c)            ARRGB( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c) \
                                            )

#define ARFLOATRGB_OF_ARGREYALPHA(_c)       ARFLOATRGB( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c) \
                                            )

#define ARRGBA_OF_ARGREYALPHA(_c)           ARRGBA( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_A(_c) \
                                            )

#define ARFLOATRGBA_OF_ARGREYALPHA(_c)      ARFLOATRGBA( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_A(_c) \
                                            )

#define ARRGB24_OF_ARGREYALPHA(_c)          ARRGB24( \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARGREYALPHA(_c)         ARRGBA32( \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARGREYALPHA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGB48_OF_ARGREYALPHA(_c)          ARRGB48( \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARGREYALPHA(_c)         ARRGBA64( \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARGREYALPHA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB96_OF_ARGREYALPHA(_c)          ARRGB96( \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARGREYALPHA(_c)        ARRGBA128( \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREYALPHA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARGREYALPHA_A(_c) * INTCOL_I32_OF_DBL \
                                            )


// =====   ArGrey8   =========================================================


#define ARGREY_OF_ARGREY8(_c)               ARGREY( \
                                            (_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREYALPHA_OF_ARGREY8(_c)          ARGREYALPHA( \
                                            (_c) * INTCOL_DBL_OF_I8,\
                                            1.0 \
                                            )

#define ARGREY16_OF_ARGREY8(_c)             ARGREY16( \
                                            (_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARGREYALPHA16_OF_ARGREY8(_c)        ARGREYALPHA16( \
                                            (_c), \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARGREY8(_c)        ARGREYALPHA32( \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARGREY8(_c)                ARRGB( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGB_OF_ARGREY8(_c)           ARFLOATRGB( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGBA_OF_ARGREY8(_c)               ARRGBA( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARGREY8(_c)           ARFLOATRGBA( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARGREY8(_c)              ARRGB24(  \
                                            (_c), \
                                            (_c), \
                                            (_c)\
                                            )

#define ARRGBA32_OF_ARGREY8(_c)             ARRGBA32(  \
                                            (_c), \
                                            (_c), \
                                            (_c), \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARGREY8(_c)              ARRGB48( \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            (_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGBA64_OF_ARGREY8(_c)             ARRGBA64( \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            (_c) * INTCOL_I16_OF_I8, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARGREY8(_c)              ARRGB96( \
                                            (_c) * INTCOL_I32_OF_I8, \
                                            (_c) * INTCOL_I32_OF_I8, \
                                            (_c) * INTCOL_I32_OF_I8 \
                                            )

#define ARRGBA128_OF_ARGREY8(_c)            ARRGBA128( \
                                            (_c) * INTCOL_I32_OF_I8, \
                                            (_c) * INTCOL_I32_OF_I8, \
                                            (_c) * INTCOL_I32_OF_I8, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArGrey16   ========================================================


#define ARGREY_OF_ARGREY16(_c)              ARGREY( \
                                            (_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREYALPHA_OF_ARGREY16(_c)         ARGREYALPHA( \
                                            (_c) * INTCOL_I8_OF_I16,\
                                            1.0 \
                                            )

#define ARGREY8_OF_ARGREY16(_c)             ARGREY8( \
                                            (_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARGREYALPHA16_OF_ARGREY16(_c)       ARGREYALPHA16( \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARGREY16(_c)       ARGREYALPHA32( \
                                            (_c), \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARGREY16(_c)               ARRGB( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGB_OF_ARGREY16(_c)          ARFLOATRGB( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGBA_OF_ARGREY16(_c)              ARRGBA( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            1.0 )

#define ARFLOATRGBA_OF_ARGREY16(_c)          ARFLOATRGBA( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARGREY16(_c)             ARRGB24( \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            (_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGBA32_OF_ARGREY16(_c)            ARRGBA32( \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            (_c) * INTCOL_I8_OF_I16, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARGREY16(_c)             ARRGB48(  \
                                            (_c), \
                                            (_c), \
                                            (_c) \
                                            )

#define ARRGBA64_OF_ARGREY16(_c)            ARRGBA64(  \
                                            (_c), \
                                            (_c), \
                                            (_c), \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARGREY16(_c)             ARRGB96( \
                                            (_c) * INTCOL_I32_OF_I16, \
                                            (_c) * INTCOL_I32_OF_I16, \
                                            (_c) * INTCOL_I32_OF_I16 \
                                            )

#define ARRGBA128_OF_ARGREY16(_c)           ARRGBA128( \
                                            (_c) * INTCOL_I32_OF_I16, \
                                            (_c) * INTCOL_I32_OF_I16, \
                                            (_c) * INTCOL_I32_OF_I16, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArGreyAlpha16   ===================================================


#define ARGREY_OF_ARGREYALPHA16(_c)         ARGREY( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREYALPHA_OF_ARGREYALPHA16(_c)    ARGREYALPHA( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8,\
                                            ARGREYALPHA16_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREY8_OF_ARGREYALPHA16(_c)        ARGREY8( ARGREYALPHA16_G(_c) )

#define ARGREY16_OF_ARGREYALPHA16(_c)       ARGREY16( ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8 )

#define ARGREYALPHA32_OF_ARGREYALPHA16(_c)  ARGREYALPHA32( \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGB_OF_ARGREYALPHA16(_c)          ARRGB( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGB_OF_ARGREYALPHA16(_c)     ARFLOATRGB( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGBA_OF_ARGREYALPHA16(_c)         ARRGBA( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGBA_OF_ARGREYALPHA16(_c)    ARFLOATRGBA( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGB24_OF_ARGREYALPHA16(_c)        ARRGB24(  \
                                            ARGREYALPHA16_G(_c), \
                                            ARGREYALPHA16_G(_c), \
                                            ARGREYALPHA16_G(_c) \
                                            )

#define ARRGBA32_OF_ARGREYALPHA16(_c)       ARRGBA32(  \
                                            ARGREYALPHA16_G(_c), \
                                            ARGREYALPHA16_G(_c), \
                                            ARGREYALPHA16_G(_c), \
                                            ARGREYALPHA16_A(_c) \
                                            )

#define ARRGB48_OF_ARGREYALPHA16(_c)        ARRGB48( \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGBA64_OF_ARGREYALPHA16(_c)       ARRGBA64( \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I16_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGB96_OF_ARGREYALPHA16(_c)        ARRGB96( \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8 \
                                            )

#define ARRGBA128_OF_ARGREYALPHA16(_c)      ARRGBA128( \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_I32_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_I32_OF_I8 \
                                            )


// =====   ArGreyAlpha32   ===================================================


#define ARGREY_OF_ARGREYALPHA32(_c)         ARGREY( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREYALPHA_OF_ARGREYALPHA32(_c)    ARGREYALPHA( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16,\
                                            ARGREYALPHA32_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREY8_OF_ARGREYALPHA32(_c)        ARGREY8( ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16 )

#define ARGREY16_OF_ARGREYALPHA32(_c)       ARGREY16( ARGREYALPHA32_G(_c) )

#define ARGREYALPHA16_OF_ARGREYALPHA32(_c)  ARGREYALPHA16( \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGB_OF_ARGREYALPHA32(_c)          ARRGB( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGB_OF_ARGREYALPHA32(_c)     ARFLOATRGB( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGBA_OF_ARGREYALPHA32(_c)         ARRGBA( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGBA_OF_ARGREYALPHA32(_c)    ARFLOATRGBA( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGB24_OF_ARGREYALPHA32(_c)        ARRGB24( \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGBA32_OF_ARGREYALPHA32(_c)       ARRGBA32( \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I8_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGB48_OF_ARGREYALPHA32(_c)        ARRGB48(  \
                                            ARGREYALPHA32_G(_c), \
                                            ARGREYALPHA32_G(_c), \
                                            ARGREYALPHA32_G(_c) \
                                            )

#define ARRGBA64_OF_ARGREYALPHA32(_c)       ARRGBA64(  \
                                            ARGREYALPHA32_G(_c), \
                                            ARGREYALPHA32_G(_c), \
                                            ARGREYALPHA32_G(_c), \
                                            ARGREYALPHA32_A(_c) \
                                            )

#define ARRGB96_OF_ARGREYALPHA32(_c)        ARRGB96( \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16 \
                                            )

#define ARRGBA128_OF_ARGREYALPHA32(_c)      ARRGBA128( \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_I32_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_I32_OF_I16 \
                                            )


// =====   ArCIEXYZ   ========================================================


#define ARCIEXYZA_OF_ARCIEXYZ(_c)           ARCIEXYZA( \
                                            ARCIEXYZ_X(_c), \
                                            ARCIEXYZ_Y(_c), \
                                            ARCIEXYZ_Z(_c), \
                                            1.0 \
                                            )

#define ARFLOATCIEXYZA_OF_ARCIEXYZ(_c)      ARFLOATCIEXYZA( \
                                            ARCIEXYZ_X(_c), \
                                            ARCIEXYZ_Y(_c), \
                                            ARCIEXYZ_Z(_c) \
                                            )

// =====   ArCIEXYZA   =======================================================


#define ARCIEXYZ_OF_ARCIEXYZA(_c)           ARCIEXYZ( \
                                            ARCIEXYZA_X(_c), \
                                            ARCIEXYZA_Y(_c), \
                                            ARCIEXYZA_Z(_c) \
                                            )

#define ARFLOATCIEXYZ_OF_ARCIEXYZA(_c)      ARFLOATCIEXYZ( \
                                            ARCIEXYZA_X(_c), \
                                            ARCIEXYZA_Y(_c), \
                                            ARCIEXYZA_Z(_c) \
                                            )

// =====   ArRGB   ===========================================================


#define ARGREY_OF_ARRGB(_c)                 ARGREY( \
                                            ARRGB_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARRGB(_c)            ARGREYALPHA( \
                                            ARRGB_G(_c), \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARRGB(_c)                ARGREY8( \
                                            ARRGB_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARRGB(_c)               ARGREY16( \
                                            ARRGB_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARRGB(_c)          ARGREYALPHA16( \
                                            ARRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARRGB(_c)          ARGREYALPHA32( \
                                            ARRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARRGB(_c)                  ARRGB( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c) \
                                            )

#define ARFLOATRGB_OF_ARRGB(_c)             ARFLOATRGB( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c) \
                                            )

#define ARRGBA_OF_ARRGB(_c)                 ARRGBA( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGBA_OF_ARRGB(_c)                 ARRGBA( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c), \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARRGB(_c)            ARFLOATRGBA( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARRGB(_c)                ARRGB24( \
                                            ARRGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARRGB(_c)               ARRGBA32( \
                                            ARRGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARRGB(_c)                ARRGB48( \
                                            ARRGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARRGB(_c)               ARRGBA64( \
                                            ARRGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARRGB(_c)                ARRGB96( \
                                            ARRGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARRGB(_c)              ARRGBA128( \
                                            ARRGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGB_B(_c) * INTCOL_I32_OF_DBL, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArFLOATRGB   =======================================================


#define ARGREY_OF_ARFLOATRGB(_c)            ARGREY( \
                                            ARFLOATRGB_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARFLOATRGB(_c)       ARGREYALPHA( \
                                            ARFLOATRGB_G(_c), \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARFLOATRGB(_c)           ARGREY8( \
                                            ARFLOATRGB_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARFLOATRGB(_c)          ARGREY16( \
                                            ARFLOATRGB_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARFLOATRGB(_c)     ARGREYALPHA16( \
                                            ARFLOATRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARFLOATRGB(_c)     ARGREYALPHA32( \
                                            ARFLOATRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARFLOATRGB(_c)             ARRGB( \
                                            ARFLOATRGB_R(_c), \
                                            ARFLOATRGB_G(_c), \
                                            ARFLOATRGB_B(_c) \
                                            )

#define ARRGB_OF_ARFLOATRGB(_c)             ARRGB( \
                                            ARFLOATRGB_R(_c), \
                                            ARFLOATRGB_G(_c), \
                                            ARFLOATRGB_B(_c) \
                                            )

#define ARRGBA_OF_ARFLOATRGB(_c)            ARRGBA( \
                                            ARFLOATRGB_R(_c), \
                                            ARFLOATRGB_G(_c), \
                                            ARFLOATRGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGBA_OF_ARFLOATRGB(_c)            ARRGBA( \
                                            ARFLOATRGB_R(_c), \
                                            ARFLOATRGB_G(_c), \
                                            ARFLOATRGB_B(_c), \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARFLOATRGB(_c)       ARFLOATRGBA( \
                                            ARFLOATRGB_R(_c), \
                                            ARFLOATRGB_G(_c), \
                                            ARFLOATRGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARFLOATRGB(_c)           ARRGB24( \
                                            ARFLOATRGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARFLOATRGB(_c)          ARRGBA32( \
                                            ARFLOATRGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARFLOATRGB(_c)           ARRGB48( \
                                            ARFLOATRGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARFLOATRGB(_c)          ARRGBA64( \
                                            ARFLOATRGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARFLOATRGB(_c)           ARRGB96( \
                                            ARFLOATRGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARFLOATRGB(_c)         ARRGBA128( \
                                            ARFLOATRGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGB_B(_c) * INTCOL_I32_OF_DBL, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArRGBA   ==========================================================


#define ARGREY_OF_ARRGBA(_c)                ARGREY( \
                                            ARRGBA_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARRGBA(_c)           ARGREYALPHA( \
                                            ARRGBA_G(_c), \
                                            ARRGBA_A(_c) \
                                            )

#define ARGREY8_OF_ARRGBA(_c)               ARGREY8( \
                                            ARRGBA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARRGBA(_c)              ARGREY16( \
                                            ARRGBA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARRGBA(_c)         ARGREYALPHA16( \
                                            ARRGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREYALPHA32_OF_ARRGBA(_c)         ARGREYALPHA32( \
                                            ARRGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB_OF_ARRGBA(_c)                 ARRGB( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c) \
                                            )

#define ARFLOATRGB_OF_ARRGBA(_c)            ARFLOATRGB( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c) \
                                            )

#define ARRGBA_OF_ARRGBA(_c)                ARRGBA( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c), \
                                            ARRGBA_A(_c) \
                                            )

#define ARFLOATRGBA_OF_ARRGBA(_c)           ARFLOATRGBA( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c), \
                                            ARRGBA_A(_c) \
                                            )

#define ARRGB24_OF_ARRGBA(_c)               ARRGB24( \
                                            ARRGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARRGBA(_c)              ARRGBA32( \
                                            ARRGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I8_OF_DBL, \
                                            ARRGBA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGB48_OF_ARRGBA(_c)               ARRGB48( \
                                            ARRGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARRGBA(_c)              ARRGBA64( \
                                            ARRGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I16_OF_DBL, \
                                            ARRGBA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB96_OF_ARRGBA(_c)               ARRGB96( \
                                            ARRGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARRGBA(_c)             ARRGBA128( \
                                            ARRGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGBA_B(_c) * INTCOL_I32_OF_DBL, \
                                            ARRGBA_A(_c) * INTCOL_I32_OF_DBL \
                                            )


// =====   ArFLOATRGBA   ======================================================


#define ARGREY_OF_ARFLOATRGBA(_c)           ARGREY( \
                                            ARFLOATRGBA_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARFLOATRGBA(_c)      ARGREYALPHA( \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_A(_c) \
                                            )

#define ARGREY8_OF_ARFLOATRGBA(_c)          ARGREY8( \
                                            ARFLOATRGBA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARFLOATRGBA(_c)         ARGREY16( \
                                            ARFLOATRGBA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARFLOATRGBA(_c)    ARGREYALPHA16( \
                                            ARFLOATRGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGB_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREYALPHA32_OF_ARFLOATRGBA(_c)    ARGREYALPHA32( \
                                            ARFLOATRGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGB_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB_OF_ARFLOATRGBA(_c)            ARRGB( \
                                            ARFLOATRGBA_R(_c), \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_B(_c) \
                                            )

#define ARRGB_OF_ARFLOATRGBA(_c)            ARRGB( \
                                            ARFLOATRGBA_R(_c), \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_B(_c) \
                                            )

#define ARFLOATRGB_OF_ARFLOATRGBA(_c)       ARFLOATRGB( \
                                            ARFLOATRGBA_R(_c), \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_B(_c) \
                                            )

#define ARRGBA_OF_ARFLOATRGBA(_c)           ARRGBA( \
                                            ARFLOATRGBA_R(_c), \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_B(_c), \
                                            ARFLOATRGBA_A(_c) \
                                            )

#define ARRGBA_OF_ARFLOATRGBA(_c)           ARRGBA( \
                                            ARFLOATRGBA_R(_c), \
                                            ARFLOATRGBA_G(_c), \
                                            ARFLOATRGBA_B(_c), \
                                            ARFLOATRGBA_A(_c) \
                                            )

#define ARRGB24_OF_ARFLOATRGBA(_c)          ARRGB24( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARFLOATRGBA(_c)         ARRGBA32( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I8_OF_DBL, \
                                            ARFLOATRGBA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGB48_OF_ARFLOATRGBA(_c)          ARRGB48( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARFLOATRGBA(_c)         ARRGBA64( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I16_OF_DBL, \
                                            ARFLOATRGBA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB96_OF_ARFLOATRGBA(_c)          ARRGB96( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARFLOATRGBA(_c)        ARRGBA128( \
                                            ARFLOATRGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGBA_B(_c) * INTCOL_I32_OF_DBL, \
                                            ARFLOATRGBA_A(_c) * INTCOL_I32_OF_DBL \
                                            )


// =====   ArRGB24   =========================================================


#define ARGREY_OF_ARRGB24(_c)               ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREYALPHA_OF_ARRGB24(_c)          ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARRGB24(_c)              ARGREY8( \
                                            ARRGBXX_G(_c) \
                                            )

#define ARGREY16_OF_ARRGB24(_c)             ARGREY16( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARGREYALPHA16_OF_ARRGB24(_c)        ARGREYALPHA16( \
                                            ARRGBXX_G(_c), \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARRGB24(_c)        ARGREYALPHA32( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARRGB24(_c)                ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGB_OF_ARRGB24(_c)           ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGBA_OF_ARRGB24(_c)               ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARRGB24(_c)          ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARRGBA32_OF_ARRGB24(_c)             ARRGBA32( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c), \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARRGB24(_c)              ARRGB48( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGBA64_OF_ARRGB24(_c)             ARRGBA64( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I8, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARRGB24(_c)              ARRGB96( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I8 \
                                            )

#define ARRGBA128_OF_ARRGB24(_c)            ARRGBA128( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I8, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArRGBA32   =========================================================


#define ARGREY_OF_ARRGBA32(_c)              ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREYALPHA_OF_ARRGBA32(_c)         ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARGREY8_OF_ARRGBA32(_c)             ARGREY8( \
                                            ARRGBXX_G(_c) \
                                            )

#define ARGREY16_OF_ARRGBA32(_c)            ARGREY16( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARGREYALPHA16_OF_ARRGBA32(_c)       ARGREYALPHA16( \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_A(_c) \
                                            )

#define ARGREYALPHA32_OF_ARRGBA32(_c)       ARGREYALPHA32( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGB_OF_ARRGBA32(_c)               ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGB_OF_ARRGBA32(_c)          ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGBA_OF_ARRGBA32(_c)              ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARFLOATRGBA_OF_ARRGBA32(_c)         ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARRGB24_OF_ARRGBA32(_c)             ARRGB24( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c) \
                                            )

#define ARRGB48_OF_ARRGBA32(_c)             ARRGB48( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGBA64_OF_ARRGBA32(_c)            ARRGBA64( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_I16_OF_I8 \
                                            )

#define ARRGB96_OF_ARRGBA32(_c)             ARRGB96( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I8 \
                                            )

#define ARRGBA128_OF_ARRGBA32(_c)           ARRGBA128( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_I32_OF_I8 \
                                            )


// =====   ArRGB48   =========================================================


#define ARGREY_OF_ARRGB48(_c)               ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREYALPHA_OF_ARRGB48(_c)          ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARRGB48(_c)              ARGREY8( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARGREY16_OF_ARRGB48(_c)             ARGREY16( \
                                            ARRGBXX_G(_c) \
                                            )

#define ARGREYALPHA16_OF_ARRGB48(_c)        ARGREYALPHA16( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARRGB48(_c)        ARGREYALPHA32( \
                                            ARRGBXX_G(_c), \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARRGB48(_c)                ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGB_OF_ARRGB48(_c)           ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGBA_OF_ARRGB48(_c)               ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            1.0 \
                                            )

#define ARFLOATRGBA_OF_ARRGB48(_c)          ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARRGB48(_c)              ARRGB24( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGBA32_OF_ARRGB48(_c)             ARRGBA32( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I16, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGBA64_OF_ARRGB48(_c)             ARRGBA64( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c), \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARRGB48(_c)              ARRGB96( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I16 \
                                            )

#define ARRGBA128_OF_ARRGB48(_c)            ARRGBA128( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I16, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArRGBA64   =========================================================


#define ARGREY_OF_ARRGBA64(_c)              ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREYALPHA_OF_ARRGBA64(_c)         ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARGREY8_OF_ARRGBA64(_c)             ARGREY8( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARGREY16_OF_ARRGBA64(_c)            ARGREY16( \
                                            ARRGBXX_G(_c) \
                                            )

#define ARGREYALPHA16_OF_ARRGBA64(_c)       ARGREYALPHA16( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARGREYALPHA32_OF_ARRGBA64(_c)       ARGREYALPHA32( \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_A(_c) \
                                            )

#define ARRGB_OF_ARRGBA64(_c)               ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGB_OF_ARRGBA64(_c)          ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGBA_OF_ARRGBA64(_c)              ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARFLOATRGBA_OF_ARRGBA64(_c)         ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARRGB24_OF_ARRGBA64(_c)             ARRGB24( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGBA32_OF_ARRGBA64(_c)            ARRGBA32( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_I8_OF_I16 \
                                            )

#define ARRGB48_OF_ARRGBA64(_c)             ARRGB48( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c) \
                                            )

#define ARRGB96_OF_ARRGBA64(_c)             ARRGB96( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I16 \
                                            )

#define ARRGBA128_OF_ARRGBA64(_c)           ARRGBA128( \
                                            ARRGBXX_R(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_I32_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_I32_OF_I16 \
                                            )


// =====   ArRGB96   =========================================================


#define ARGREY_OF_ARRGB96(_c)               ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARGREYALPHA_OF_ARRGB96(_c)          ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARRGB96(_c)              ARGREY8( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARGREY16_OF_ARRGB96(_c)             ARGREY16( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARGREYALPHA16_OF_ARRGB96(_c)        ARGREYALPHA16( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARRGB96(_c)        ARGREYALPHA32( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARRGB96(_c)                ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARFLOATRGB_OF_ARRGB96(_c)           ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARRGBA_OF_ARRGB96(_c)               ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            1.0 )

#define ARFLOATRGBA_OF_ARRGB96(_c)          ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            1.0 )

#define ARRGB24_OF_ARRGB96(_c)              ARRGB24( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARRGBA32_OF_ARRGB96(_c)             ARRGBA32( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I32, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARRGB96(_c)              ARRGB48( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARRGBA64_OF_ARRGB96(_c)             ARRGBA64( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I32, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGBA128_OF_ARRGB96(_c)            ARRGBA128( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c), \
                                            ART_UINT32_MAX \
                                            )


// =====   ArRGBA128   ========================================================


#define ARGREY_OF_ARRGBA128(_c)             ARGREY( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARGREYALPHA_OF_ARRGBA128(_c)        ARGREYALPHA( \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARGREY8_OF_ARRGBA128(_c)            ARGREY8( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARGREY16_OF_ARRGBA128(_c)           ARGREY16( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARGREYALPHA16_OF_ARRGBA128(_c)      ARGREYALPHA16( \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARGREYALPHA32_OF_ARRGBA128(_c)      ARGREYALPHA32( \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARRGB_OF_ARRGBA128(_c)              ARRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARFLOATRGB_OF_ARRGBA128(_c)          ARFLOATRGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARRGBA_OF_ARRGBA128(_c)             ARRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARFLOATRGBA_OF_ARRGBA128(_c)        ARFLOATRGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARRGB24_OF_ARRGBA128(_c)            ARRGB24( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARRGBA32_OF_ARRGBA128(_c)           ARRGBA32( \
                                            ARRGBXX_R(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I8_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_I8_OF_I32 \
                                            )

#define ARRGB48_OF_ARRGBA128(_c)            ARRGB48( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARRGBA64_OF_ARRGBA128(_c)           ARRGBA64( \
                                            ARRGBXX_R(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_I16_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_I16_OF_I32 \
                                            )

#define ARRGB96_OF_ARRGBA128(_c)            ARRGB96( \
                                            ARRGBXX_R(_c), \
                                            ARRGBXX_G(_c), \
                                            ARRGBXX_B(_c) \
                                            )


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMCONVERSIONMACROS_H_ */
/* ======================================================================== */
