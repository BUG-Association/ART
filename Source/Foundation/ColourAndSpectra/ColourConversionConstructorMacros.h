/* ===========================================================================

    Copyright (c) 1996-2018 The ART Development Team
    ------------------------------------------------

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

#define ARUT_RGB_OF_ARGREY(_c)              ARUT_RGB( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c) \
                                            )

#define ARUTF_RGB_OF_ARGREY(_c)             ARUTF_RGB( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c) \
                                            )

#define ARUT_RGBA_OF_ARGREY(_c)             ARUT_RGBA( \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            ARGREY_G(_c), \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARGREY(_c)            ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARGREYALPHA(_c)         ARUT_RGB( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c) \
                                            )

#define ARUTF_RGB_OF_ARGREYALPHA(_c)        ARUTF_RGB( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c) \
                                            )

#define ARUT_RGBA_OF_ARGREYALPHA(_c)        ARUT_RGBA( \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_G(_c), \
                                            ARGREYALPHA_A(_c) \
                                            )

#define ARUTF_RGBA_OF_ARGREYALPHA(_c)       ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARGREY8(_c)             ARUT_RGB( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGB_OF_ARGREY8(_c)            ARUTF_RGB( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUT_RGBA_OF_ARGREY8(_c)            ARUT_RGBA( \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            (_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARGREY8(_c)           ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARGREY16(_c)            ARUT_RGB( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGB_OF_ARGREY16(_c)           ARUTF_RGB( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUT_RGBA_OF_ARGREY16(_c)           ARUT_RGBA( \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            (_c) * INTCOL_DBL_OF_I16, \
                                            1.0 )

#define ARUTF_RGBA_OF_ARGREY16(_c)          ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARGREYALPHA16(_c)       ARUT_RGB( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGB_OF_ARGREYALPHA16(_c)      ARUTF_RGB( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUT_RGBA_OF_ARGREYALPHA16(_c)      ARUT_RGBA( \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARGREYALPHA16_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGBA_OF_ARGREYALPHA16(_c)     ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARGREYALPHA32(_c)       ARUT_RGB( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGB_OF_ARGREYALPHA32(_c)      ARUTF_RGB( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUT_RGBA_OF_ARGREYALPHA32(_c)      ARUT_RGBA( \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARGREYALPHA32_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGBA_OF_ARGREYALPHA32(_c)     ARUTF_RGBA( \
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


// =====   ArRGB   ===========================================================


#define ARGREY_OF_ARRGB(_c)                 ARGREY_CS( \
                                            ARRGB_G(_c), \
                                            ARRGB_S(_c) \
                                            )

#define ARGREYALPHA_OF_ARRGB(_c)            ARGREYALPHA_CS( \
                                            ARRGB_G(_c), \
                                            1.0, \
                                            ARRGB_S(_c) \
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

#define ARUT_RGB_OF_ARRGB(_c)               ARUT_RGB( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c) \
                                            )

#define ARUTF_RGB_OF_ARRGB(_c)              ARUTF_RGB( \
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

#define ARUT_RGBA_OF_ARRGB(_c)              ARUT_RGBA( \
                                            ARRGB_R(_c), \
                                            ARRGB_G(_c), \
                                            ARRGB_B(_c), \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARRGB(_c)             ARUTF_RGBA( \
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


// =====   ArUT_RGB   ========================================================


#define ARGREY_OF_ARUT_RGB(_c)              ARGREY( \
                                            ARUT_RGB_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARUT_RGB(_c)         ARGREYALPHA( \
                                            ARUT_RGB_G(_c), \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARUT_RGB(_c)             ARGREY8( \
                                            ARUT_RGB_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARUT_RGB(_c)            ARGREY16( \
                                            ARUT_RGB_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARUT_RGB(_c)       ARGREYALPHA16( \
                                            ARUT_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARUT_RGB(_c)       ARGREYALPHA32( \
                                            ARUT_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARUT_RGB(_c)               ARRGB( \
                                            ARUT_RGB_R(_c), \
                                            ARUT_RGB_G(_c), \
                                            ARUT_RGB_B(_c) \
                                            )

#define ARUTF_RGB_OF_ARUT_RGB(_c)           ARUTF_RGB( \
                                            ARUT_RGB_R(_c), \
                                            ARUT_RGB_G(_c), \
                                            ARUT_RGB_B(_c) \
                                            )

#define ARRGBA_OF_ARUT_RGB(_c)              ARRGBA( \
                                            ARUT_RGB_R(_c), \
                                            ARUT_RGB_G(_c), \
                                            ARUT_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARUT_RGBA_OF_ARUT_RGB(_c)           ARUT_RGBA( \
                                            ARUT_RGB_R(_c), \
                                            ARUT_RGB_G(_c), \
                                            ARUT_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARUT_RGB(_c)          ARUTF_RGBA( \
                                            ARUT_RGB_R(_c), \
                                            ARUT_RGB_G(_c), \
                                            ARUT_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARUT_RGB(_c)             ARRGB24( \
                                            ARUT_RGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARUT_RGB(_c)            ARRGBA32( \
                                            ARUT_RGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARUT_RGB(_c)             ARRGB48( \
                                            ARUT_RGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARUT_RGB(_c)            ARRGBA64( \
                                            ARUT_RGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARUT_RGB(_c)             ARRGB96( \
                                            ARUT_RGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARUT_RGB(_c)           ARRGBA128( \
                                            ARUT_RGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGB_B(_c) * INTCOL_I32_OF_DBL, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArUTF_RGB   =======================================================


#define ARGREY_OF_ARUTF_RGB(_c)             ARGREY( \
                                            ARUTF_RGB_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARUTF_RGB(_c)        ARGREYALPHA( \
                                            ARUTF_RGB_G(_c), \
                                            1.0 \
                                            )

#define ARGREY8_OF_ARUTF_RGB(_c)            ARGREY8( \
                                            ARUTF_RGB_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARUTF_RGB(_c)           ARGREY16( \
                                            ARUTF_RGB_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARUTF_RGB(_c)      ARGREYALPHA16( \
                                            ARUTF_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARGREYALPHA32_OF_ARUTF_RGB(_c)      ARGREYALPHA32( \
                                            ARUTF_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB_OF_ARUTF_RGB(_c)              ARRGB( \
                                            ARUTF_RGB_R(_c), \
                                            ARUTF_RGB_G(_c), \
                                            ARUTF_RGB_B(_c) \
                                            )

#define ARUT_RGB_OF_ARUTF_RGB(_c)           ARUT_RGB( \
                                            ARUTF_RGB_R(_c), \
                                            ARUTF_RGB_G(_c), \
                                            ARUTF_RGB_B(_c) \
                                            )

#define ARRGBA_OF_ARUTF_RGB(_c)             ARRGBA( \
                                            ARUTF_RGB_R(_c), \
                                            ARUTF_RGB_G(_c), \
                                            ARUTF_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARUT_RGBA_OF_ARUTF_RGB(_c)          ARUT_RGBA( \
                                            ARUTF_RGB_R(_c), \
                                            ARUTF_RGB_G(_c), \
                                            ARUTF_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARUTF_RGB(_c)         ARUTF_RGBA( \
                                            ARUTF_RGB_R(_c), \
                                            ARUTF_RGB_G(_c), \
                                            ARUTF_RGB_B(_c), \
                                            1.0 \
                                            )

#define ARRGB24_OF_ARUTF_RGB(_c)            ARRGB24( \
                                            ARUTF_RGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARUTF_RGB(_c)           ARRGBA32( \
                                            ARUTF_RGB_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I8_OF_DBL, \
                                            ART_UINT8_MAX \
                                            )

#define ARRGB48_OF_ARUTF_RGB(_c)            ARRGB48( \
                                            ARUTF_RGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARUTF_RGB(_c)           ARRGBA64( \
                                            ARUTF_RGB_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I16_OF_DBL, \
                                            ART_UINT16_MAX \
                                            )

#define ARRGB96_OF_ARUTF_RGB(_c)            ARRGB96( \
                                            ARUTF_RGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARUTF_RGB(_c)          ARRGBA128( \
                                            ARUTF_RGB_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGB_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGB_B(_c) * INTCOL_I32_OF_DBL, \
                                            ART_UINT32_MAX \
                                            )


// =====   ArRGBA   ==========================================================


#define ARGREY_OF_ARRGBA(_c)                ARGREY_CS( \
                                            ARRGBA_G(_c) , \
                                            ARRGBA_S(_c) \
                                            )

#define ARGREYALPHA_OF_ARRGBA(_c)           ARGREYALPHA_CS( \
                                            ARRGBA_G(_c), \
                                            ARRGBA_A(_c), \
                                            ARRGBA_S(_c) \
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

#define ARRGB_OF_ARRGBA(_c)                 ARRGB_CS( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c), \
                                            ARRGBA_S(_c) \
                                            )

#define ARUT_RGB_OF_ARRGBA(_c)              ARUT_RGB( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c) \
                                            )

#define ARUTF_RGB_OF_ARRGBA(_c)             ARUTF_RGB( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c) \
                                            )

#define ARUT_RGBA_OF_ARRGBA(_c)             ARUT_RGBA( \
                                            ARRGBA_R(_c), \
                                            ARRGBA_G(_c), \
                                            ARRGBA_B(_c), \
                                            ARRGBA_A(_c) \
                                            )

#define ARUTF_RGBA_OF_ARRGBA(_c)            ARUTF_RGBA( \
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


// =====   ArUT_RGBA   =======================================================


#define ARGREY_OF_ARUT_RGBA(_c)             ARGREY( \
                                            ARUT_RGBA_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARUT_RGBA(_c)        ARGREYALPHA( \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_A(_c) \
                                            )

#define ARGREY8_OF_ARUT_RGBA(_c)            ARGREY8( \
                                            ARUT_RGBA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARUT_RGBA(_c)           ARGREY16( \
                                            ARUT_RGBA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARUT_RGBA(_c)      ARGREYALPHA16( \
                                            ARUT_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGB_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREYALPHA32_OF_ARUT_RGBA(_c)      ARGREYALPHA32( \
                                            ARUT_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGB_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB_OF_ARUT_RGBA(_c)              ARRGB( \
                                            ARUT_RGBA_R(_c), \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_B(_c) \
                                            )

#define ARUT_RGB_OF_ARUT_RGBA(_c)           ARUT_RGB( \
                                            ARUT_RGBA_R(_c), \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_B(_c) \
                                            )

#define ARUTF_RGB_OF_ARUT_RGBA(_c)          ARUTF_RGB( \
                                            ARUT_RGBA_R(_c), \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_B(_c) \
                                            )

#define ARRGBA_OF_ARUT_RGBA(_c)             ARRGBA( \
                                            ARUT_RGBA_R(_c), \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_B(_c), \
                                            ARUT_RGBA_A(_c) \
                                            )

#define ARUTF_RGBA_OF_ARUT_RGBA(_c)         ARUTF_RGBA( \
                                            ARUT_RGBA_R(_c), \
                                            ARUT_RGBA_G(_c), \
                                            ARUT_RGBA_B(_c), \
                                            ARUT_RGBA_A(_c) \
                                            )

#define ARRGB24_OF_ARUT_RGBA(_c)            ARRGB24( \
                                            ARUT_RGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARUT_RGBA(_c)           ARRGBA32( \
                                            ARUT_RGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I8_OF_DBL, \
                                            ARUT_RGBA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGB48_OF_ARUT_RGBA(_c)            ARRGB48( \
                                            ARUT_RGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARUT_RGBA(_c)           ARRGBA64( \
                                            ARUT_RGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I16_OF_DBL, \
                                            ARUT_RGBA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB96_OF_ARUT_RGBA(_c)            ARRGB96( \
                                            ARUT_RGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARUT_RGBA(_c)          ARRGBA128( \
                                            ARUT_RGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGBA_B(_c) * INTCOL_I32_OF_DBL, \
                                            ARUT_RGBA_A(_c) * INTCOL_I32_OF_DBL \
                                            )


// =====   ArUTF_RGBA   ======================================================


#define ARGREY_OF_ARUTF_RGBA(_c)            ARGREY( \
                                            ARUTF_RGBA_G(_c) \
                                            )

#define ARGREYALPHA_OF_ARUTF_RGBA(_c)       ARGREYALPHA( \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_A(_c) \
                                            )

#define ARGREY8_OF_ARUTF_RGBA(_c)           ARGREY8( \
                                            ARUTF_RGBA_G(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREY16_OF_ARUTF_RGBA(_c)          ARGREY16( \
                                            ARUTF_RGBA_G(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARGREYALPHA16_OF_ARUTF_RGBA(_c)     ARGREYALPHA16( \
                                            ARUTF_RGB_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGB_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARGREYALPHA32_OF_ARUTF_RGBA(_c)     ARGREYALPHA32( \
                                            ARUTF_RGB_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGB_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB_OF_ARUTF_RGBA(_c)             ARRGB( \
                                            ARUTF_RGBA_R(_c), \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_B(_c) \
                                            )

#define ARUT_RGB_OF_ARUTF_RGBA(_c)          ARUT_RGB( \
                                            ARUTF_RGBA_R(_c), \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_B(_c) \
                                            )

#define ARUTF_RGB_OF_ARUTF_RGBA(_c)         ARUTF_RGB( \
                                            ARUTF_RGBA_R(_c), \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_B(_c) \
                                            )

#define ARRGBA_OF_ARUTF_RGBA(_c)            ARRGBA( \
                                            ARUTF_RGBA_R(_c), \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_B(_c), \
                                            ARUTF_RGBA_A(_c) \
                                            )

#define ARUT_RGBA_OF_ARUTF_RGBA(_c)         ARUT_RGBA( \
                                            ARUTF_RGBA_R(_c), \
                                            ARUTF_RGBA_G(_c), \
                                            ARUTF_RGBA_B(_c), \
                                            ARUTF_RGBA_A(_c) \
                                            )

#define ARRGB24_OF_ARUTF_RGBA(_c)           ARRGB24( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGBA32_OF_ARUTF_RGBA(_c)          ARRGBA32( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I8_OF_DBL, \
                                            ARUTF_RGBA_A(_c) * INTCOL_I8_OF_DBL \
                                            )

#define ARRGB48_OF_ARUTF_RGBA(_c)           ARRGB48( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGBA64_OF_ARUTF_RGBA(_c)          ARRGBA64( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I16_OF_DBL, \
                                            ARUTF_RGBA_A(_c) * INTCOL_I16_OF_DBL \
                                            )

#define ARRGB96_OF_ARUTF_RGBA(_c)           ARRGB96( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I32_OF_DBL \
                                            )

#define ARRGBA128_OF_ARUTF_RGBA(_c)         ARRGBA128( \
                                            ARUTF_RGBA_R(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGBA_G(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGBA_B(_c) * INTCOL_I32_OF_DBL, \
                                            ARUTF_RGBA_A(_c) * INTCOL_I32_OF_DBL \
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

#define ARUT_RGB_OF_ARRGB24(_c)             ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGB_OF_ARRGB24(_c)            ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUT_RGBA_OF_ARRGB24(_c)            ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARRGB24(_c)           ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARRGBA32(_c)            ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGB_OF_ARRGBA32(_c)           ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUT_RGBA_OF_ARRGBA32(_c)           ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I8, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I8 \
                                            )

#define ARUTF_RGBA_OF_ARRGBA32(_c)          ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARRGB48(_c)             ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGB_OF_ARRGB48(_c)            ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUT_RGBA_OF_ARRGB48(_c)            ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            1.0 \
                                            )

#define ARUTF_RGBA_OF_ARRGB48(_c)           ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARRGBA64(_c)            ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGB_OF_ARRGBA64(_c)           ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUT_RGBA_OF_ARRGBA64(_c)           ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I16, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I16 \
                                            )

#define ARUTF_RGBA_OF_ARRGBA64(_c)          ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARRGB96(_c)             ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARUTF_RGB_OF_ARRGB96(_c)            ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARUT_RGBA_OF_ARRGB96(_c)            ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            1.0 )

#define ARUTF_RGBA_OF_ARRGB96(_c)           ARUTF_RGBA( \
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

#define ARUT_RGB_OF_ARRGBA128(_c)           ARUT_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARUTF_RGB_OF_ARRGBA128(_c)          ARUTF_RGB( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARUT_RGBA_OF_ARRGBA128(_c)          ARUT_RGBA( \
                                            ARRGBXX_R(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_G(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_B(_c) * INTCOL_DBL_OF_I32, \
                                            ARRGBXX_A(_c) * INTCOL_DBL_OF_I32 \
                                            )

#define ARUTF_RGBA_OF_ARRGBA128(_c)         ARUTF_RGBA( \
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
