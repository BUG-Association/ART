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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTYPE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTYPE_H_

#include "ART_Foundation_System.h"

#define ARSPECTRUM_NUMCHANNELS(__act) \
    (((__act) & arspectrum_channels_mask ) >> 8 )

typedef enum ArSpectrumType
{
    arspectrum_unknown                = 0,

    arspectrum_bits_mask              = 0x000000ff,
    arspectrum_channels_mask          = 0x000fff00,

    arspectrum_uint                   = 0x00100000,
    arspectrum_float                  = 0x00200000,
    arspectrum_coded                  = 0x00400000,
    arspectrum_alpha                  = 0x00800000,
    arspectrum_xyz                    = 0x01000000,
    arspectrum_negative               = 0x02000000,
    arspectrum_cmyk                   = 0x04000000,
    arspectrum_polarisable            = 0x08000000,
    arspectrum_untagged               = 0x10000000,
    arspectrum_falsecolour            = 0x20000000,
    arspectrum_plusminus              = 0x40000000,

    arspectrum_grey1                  = 0x00101 | arspectrum_uint,
    arspectrum_grey2                  = 0x00102 | arspectrum_uint,
    arspectrum_grey4                  = 0x00104 | arspectrum_uint,
    arspectrum_grey8                  = 0x00108 | arspectrum_uint,
    arspectrum_grey16                 = 0x00110 | arspectrum_uint,
    arspectrum_grey16alpha            = 0x00110 | arspectrum_uint | arspectrum_alpha,
    arspectrum_grey32                 = 0x00120 | arspectrum_uint,
    arspectrum_grey32alpha            = 0x00120 | arspectrum_uint | arspectrum_alpha,
    arspectrum_grey64                 = 0x00140 | arspectrum_uint,
    arspectrum_grey64alpha            = 0x00140 | arspectrum_uint | arspectrum_alpha,
    arspectrum_grey1_negative         = 0x00101 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey2_negative         = 0x00102 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey4_negative         = 0x00104 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey8_negative         = 0x00108 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey16_negative        = 0x00110 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey32_negative        = 0x00120 | arspectrum_uint | arspectrum_negative,
    arspectrum_grey64_negative        = 0x00140 | arspectrum_uint | arspectrum_negative,
    arspectrum_rgb12                  = 0x00304 | arspectrum_uint,
    arspectrum_rgb24                  = 0x00308 | arspectrum_uint,
    arspectrum_rgb24falsecolour       = 0x00408 | arspectrum_uint | arspectrum_falsecolour,
    arspectrum_rgb48                  = 0x00310 | arspectrum_uint,
    arspectrum_rgb48falsecolour       = 0x00410 | arspectrum_uint | arspectrum_falsecolour,
    arspectrum_rgb96                  = 0x00320 | arspectrum_uint,
    arspectrum_rgb192                 = 0x00340 | arspectrum_uint,
    arspectrum_rgb16                  = 0x00404 | arspectrum_uint | arspectrum_alpha,
    arspectrum_rgba32                 = 0x00408 | arspectrum_uint | arspectrum_alpha,
    arspectrum_rgba32falsecolour      = 0x00408 | arspectrum_uint | arspectrum_alpha | arspectrum_falsecolour,
    arspectrum_rgba32plusminus        = 0x00408 | arspectrum_uint | arspectrum_alpha | arspectrum_plusminus,
    arspectrum_rgba64                 = 0x00410 | arspectrum_uint | arspectrum_alpha,
    arspectrum_rgba64falsecolour      = 0x00410 | arspectrum_uint | arspectrum_alpha | arspectrum_falsecolour,
    arspectrum_rgba64plusminus        = 0x00410 | arspectrum_uint | arspectrum_alpha | arspectrum_plusminus,
    arspectrum_rgba128                = 0x00420 | arspectrum_uint | arspectrum_alpha,
    arspectrum_rgb256                 = 0x00440 | arspectrum_uint | arspectrum_alpha,
    arspectrum_fgrey                  = 0x00120 | arspectrum_float,
    arspectrum_grey                   = 0x00140 | arspectrum_float,
    arspectrum_greyalpha              = 0x00140 | arspectrum_float | arspectrum_alpha,
    arspectrum_frgb                   = 0x00320 | arspectrum_float,
    arspectrum_rgb                    = 0x00340 | arspectrum_float,
    arspectrum_ut_rgb                 = 0x00340 | arspectrum_float | arspectrum_untagged,
    arspectrum_ut_rgb_polarisable     = 0x00340 | arspectrum_float | arspectrum_untagged
                                      | arspectrum_polarisable,
    arspectrum_frgba                  = 0x00420 | arspectrum_float | arspectrum_alpha,
    arspectrum_rgba                   = 0x00440 | arspectrum_float | arspectrum_alpha,
    arspectrum_ut_rgba                = 0x00440
                                      | arspectrum_float | arspectrum_alpha
                                      | arspectrum_untagged,
    arspectrum_fciexyz                = 0x00320 | arspectrum_float | arspectrum_xyz,
    arspectrum_ciexyz                 = 0x00340 | arspectrum_float | arspectrum_xyz,
    arspectrum_ciexyz_polarisable     = 0x00340 | arspectrum_float | arspectrum_xyz
                                      | arspectrum_polarisable,
    arspectrum_fciexyza               = 0x00420
                                      | arspectrum_float | arspectrum_xyz | arspectrum_alpha,
    arspectrum_ciexyza                = 0x00440
                                      | arspectrum_float | arspectrum_xyz | arspectrum_alpha,

    //   Hidden gotcha: the 'arspectrum' type encodes the number of channels.
    //                  Due to being quite a fragile duplicate solution, this
    //                  will be removed in the foreseeable future. Right now,
    //                  the ARTRAW code still uses this.
    
    //                                     ||
    //               specifically, these   vv   bits encode the #channels
    
    arspectrum_fspectrum8             = 0x00820 | arspectrum_float,
    arspectrum_spectrum8              = 0x00840 | arspectrum_float,
    arspectrum_spectrum8_polarisable  = 0x00840 | arspectrum_float | arspectrum_polarisable,
    arspectrum_fspectrum11            = 0x00b20 | arspectrum_float,
    arspectrum_spectrum11             = 0x00b40 | arspectrum_float,
    arspectrum_spectrum11_polarisable = 0x00b40 | arspectrum_float | arspectrum_polarisable,
    arspectrum_fspectrum18            = 0x01220 | arspectrum_float,
    arspectrum_spectrum18             = 0x01240 | arspectrum_float,
    arspectrum_spectrum18_polarisable = 0x01240 | arspectrum_float | arspectrum_polarisable,
    arspectrum_fspectrum46            = 0x02e20 | arspectrum_float,
    arspectrum_spectrum46             = 0x02e40 | arspectrum_float,
    arspectrum_spectrum46_polarisable = 0x02e40 | arspectrum_float | arspectrum_polarisable,
    
    arspectrum_logluv                 = 0x00120 | arspectrum_coded | arspectrum_xyz,
    arspectrum_all_bits_set           = 0xFFFFF,
}
ArSpectrumType;

typedef     ArSpectrumType              ArColourType;

#define     arcolour_bits_mask          arspectrum_bits_mask
#define     arcolour_channels_mask      arspectrum_channels_mask

#define     arcolour_uint               arspectrum_uint
#define     arcolour_float              arspectrum_float
#define     arcolour_coded              arspectrum_coded
#define     arcolour_alpha              arspectrum_alpha
#define     arcolour_xyz                arspectrum_xyz
#define     arcolour_negative           arspectrum_negative
#define     arcolour_cmyk               arspectrum_cmyk
#define     arcolour_polarisable        arspectrum_polarisable
#define     arcolour_untagged           arspectrum_untagged
#define     arcolour_falsecolour        arspectrum_falsecolour

#define     arcolour_unknown            arspectrum_unknown
#define     arcolour_grey1              arspectrum_grey1
#define     arcolour_grey2              arspectrum_grey2
#define     arcolour_grey4              arspectrum_grey4
#define     arcolour_grey8              arspectrum_grey8
#define     arcolour_grey16             arspectrum_grey16
#define     arcolour_grey16alpha        arspectrum_grey16alpha
#define     arcolour_grey32             arspectrum_grey32
#define     arcolour_grey32alpha        arspectrum_grey32alpha
#define     arcolour_grey64             arspectrum_grey64
#define     arcolour_grey64alpha        arspectrum_grey64alpha
#define     arcolour_grey1_negative     arspectrum_grey1_negative
#define     arcolour_grey2_negative     arspectrum_grey2_negative
#define     arcolour_grey4_negative     arspectrum_grey4_negative
#define     arcolour_grey8_negative     arspectrum_grey8_negative
#define     arcolour_grey16_negative    arspectrum_grey16_negative
#define     arcolour_grey32_negative    arspectrum_grey32_negative
#define     arcolour_grey64_negative    arspectrum_grey64_negative
#define     arcolour_rgb12              arspectrum_rgb12
#define     arcolour_rgb24              arspectrum_rgb24
#define     arcolour_rgb24falsecolour   arspectrum_rgb24falsecolour
#define     arcolour_rgb48              arspectrum_rgb48
#define     arcolour_rgb48falsecolour   arspectrum_rgb48falsecolour
#define     arcolour_rgb96              arspectrum_rgb96
#define     arcolour_rgb192             arspectrum_rgb192
#define     arcolour_rgb16              arspectrum_rgb16
#define     arcolour_rgba32             arspectrum_rgba32
#define     arcolour_rgba32falsecolour  arspectrum_rgba32falsecolour
#define     arcolour_rgba64             arspectrum_rgba64
#define     arcolour_rgba64falsecolour  arspectrum_rgba64falsecolour
#define     arcolour_rgba128            arspectrum_rgba128
#define     arcolour_rgb256             arspectrum_rgb256
#define     arcolour_ciexyz             arspectrum_ciexyz

#define     arcolour_ut_rgb             arspectrum_ut_rgb
#define     arcolour_spectrum8          arspectrum_spectrum8
#define     arcolour_spectrum11         arspectrum_spectrum11
#define     arcolour_spectrum18         arspectrum_spectrum18
#define     arcolour_spectrum46         arspectrum_spectrum46

#define     arcolour_ut_rgb_polarisable         arspectrum_ut_rgb_polarisable
#define     arcolour_spectrum8_polarisable      arspectrum_spectrum8_polarisable
#define     arcolour_spectrum11_polarisable     arspectrum_spectrum11_polarisable
#define     arcolour_spectrum18_polarisable     arspectrum_spectrum18_polarisable
#define     arcolour_spectrum46_polarisable     arspectrum_spectrum46_polarisable


const char * arspectrumtype_name(
        ArSpectrumType  spectrumType
        );

const char * arspectrumtype_long_name_string(
        ART_GV          * art_gv,
        ArSpectrumType    spectrumType
        );

const char * arspectrumtype_polarisable_string(
        ArSpectrumType  spectrumType
        );

unsigned int arspectrumtype_channels(
        ArSpectrumType  spectrumType
        );

unsigned int arspectrumtype_bits(
        ArSpectrumType  spectrumType
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARSPECTRUMTYPE_H_ */
/* ======================================================================== */
