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

#ifndef _ART_FOUNDATION_COLOURANDSPECTRA_ARDATATYPE_H_
#define _ART_FOUNDATION_COLOURANDSPECTRA_ARDATATYPE_H_

#include "ART_Foundation_System.h"

/* ---------------------------------------------------------------------------

    'ArDataType' enum
 
    Purpose of this enum is to provide information on the properties of the
    colour and spectral data types used within ART. These types are in some
    cases used in a somewhat polymorphic fashion, as there are e.g. images
    which can contain either colour values or spectra. So an enum capable of
    (for instance) telling you exactly what it is you are actually getting
    back from an image makes some sense. Specifically, information on
 
    - the size of the data type in bits
    - the number of channels
    - whether there is an alpha channel,
    - the content type (grey, RGB, XYZ, spectrum)
    - underlying data type (int, float, double)
    - and extra features (polarisation, falsecolour, etc.)
 
    is provided.
 
    Note that as the size of the data type in bits is mostly only used
    when dealing with the classical integer data types, we don't provide a
    value for the spectral data types, or the colour types which contain
    polarisation info. In these cases, we return 'ardt_bits_noinfo', which
    basically means "please figure this out by yourself".
 
    Some info on the extra features which are encoded:
 
    - polarisation, alpha: self-explanatory
 
    - negative: greyscape TIFF images have two modes: one where the value
                of "white" is encoded as 255 (in an 8 bit scenario), and
                one where it is encoded as 0 (with 255 being black). This
                flag takes care of the distinction within ART.
                (it is exceedingly unlikely that anyone will ever need this,
                but as the existing TIFF code is capable of handling this,
                it seemed pointless to remove the capability)
 
    - untagged: the standard form of ART colour data types contains three
                floats or doubles for the colour coordinates, plus a pointer
                to colourspace information. This is fine for colour handling
                purposes when dealing with a small number of colour values:
                but for rendering applications, this is overkill and a waste
                of space (one pointer per value, when all of them are the
                same in a rendered image). So there is an untagged version
                of RGB and XYZ as well.
 
    - falsecolour, plusminus: these flags are used by visualisation tools,
                to mark their RGB output as having these two features.

------------------------------------------------------------------------aw- */

//   The following file contains various #defines needed to allow a concise
//   definition of the actual enum values further down.

#include "ArDataType_PreDefinitionMacros.h"

typedef enum ArDataType
{
    ardt_unknown                = 0,

    ardt_size_in_bits_mask      = 0x000001ff,
    ardt_num_channels_mask      = 0x001ff000,
    ardt_type_mask              = 0x0f000000,
    ardt_flag_mask              = 0xf0000000,

    ardt_bits_noinfo            = 0x00000000,
    ardt_bits_8                 = 0x00000008,
    ardt_bits_16                = 0x00000010,
    ardt_bits_24                = 0x00000018,
    ardt_bits_32                = 0x00000020,
    ardt_bits_48                = 0x00000030,
    ardt_bits_64                = 0x00000040,
    ardt_bits_96                = 0x00000060,
    ardt_bits_128               = 0x00000080,
    ardt_bits_192               = 0x000000C0,
    ardt_bits_256               = 0x00000100,

    ardt_channels_1             = 0x00001000,
    ardt_channels_2             = 0x00002000,
    ardt_channels_3             = 0x00003000,
    ardt_channels_4             = 0x00004000,
    ardt_channels_8             = 0x00008000,
    ardt_channels_11            = 0x0000B000,
    ardt_channels_18            = 0x00012000,
    ardt_channels_46            = 0x0002E000,
    ardt_channels_500           = 0x001F4000,

    ardt_uint                   = 0x01000000,
    ardt_float                  = 0x02000000,
    ardt_double                 = 0x04000000,
    ardt_negative               = 0x08000000,
    
    ardt_polarisable            = 0x10000000,
//    ardt_untagged               = 0x20000000,
    ardt_falsecolour            = 0x40000000,
    ardt_plusminus              = 0x80000000,
    
    ardt_content_grey           = 0x00000200,
    ardt_content_rgb            = 0x00000400,
    ardt_content_xyz            = 0x00000800,
    ardt_content_spectrum       = 0x00200000,
    ardt_content_alpha          = 0x00400000,
    
    ardt_content_grey_alpha     = ardt_content_grey | ardt_content_alpha,
    ardt_content_rgb_alpha      = ardt_content_rgb  | ardt_content_alpha,
    ardt_content_xyz_alpha      = ardt_content_xyz  | ardt_content_alpha,

    //   The shortcuts used here are macros which are #defined in the file
    //   included right before this enum definition.
    
    ardt_grey8                  = C_1   | B_8    | INT | G,
    ardt_grey8_negative         = C_1   | B_8    | INT | G    | NEG,
    ardt_grey16                 = C_1   | B_16   | INT | G,
    ardt_grey16_negative        = C_1   | B_16   | INT | G    | NEG,
    ardt_grey16_alpha           = C_2   | B_32   | INT | GA,
    ardt_grey32                 = C_1   | B_32   | INT | G,
    ardt_grey32_negative        = C_1   | B_32   | INT | G    | NEG,
    ardt_grey32_alpha           = C_2   | B_64   | INT | GA,
    ardt_grey64                 = C_1   | B_64   | INT | G,
    ardt_grey64_negative        = C_1   | B_64   | INT | G    | NEG,
    ardt_grey64_alpha           = C_2   | B_128  | INT | GA,
    ardt_fgrey                  = C_1   | B_32   | FLT | G,
    ardt_grey                   = C_1   | B_64   | DBL | G,
    ardt_grey_alpha             = C_2   | B_128  | DBL | GA,

    ardt_rgb24                  = C_3   | B_24   | INT | RGB,
    ardt_rgb24_falsecolour      = C_3   | B_24   | INT | RGB  | FC,
    ardt_rgba32                 = C_4   | B_32   | INT | RGBA,
    ardt_rgba32_falsecolour     = C_4   | B_32   | INT | RGBA | FC,
    ardt_rgba32_plusminus       = C_4   | B_32   | INT | RGBA | PM,
    ardt_rgb48                  = C_3   | B_48   | INT | RGB,
    ardt_rgb48_falsecolour      = C_3   | B_48   | INT | RGB  | FC,
    ardt_rgba64                 = C_4   | B_64   | INT | RGBA,
    ardt_rgba64_falsecolour     = C_4   | B_64   | INT | RGBA | FC,
    ardt_rgba64_plusminus       = C_4   | B_64   | INT | RGBA | PM,
    ardt_rgb96                  = C_3   | B_96   | INT | RGB,

    ardt_frgb                   = C_3   | B_96   | FLT | RGB,
    ardt_rgb                    = C_3   | B_192  | DBL | RGB,
    ardt_rgb_polarisable        = C_3   | B_192  | DBL | RGB  | POL,
    ardt_frgba                  = C_4   | B_128  | FLT | RGBA,
    ardt_rgba                   = C_4   | B_256  | DBL | RGBA,
    ardt_rgba_polarisable       = C_4   | B_256  | DBL | RGBA | POL,

    ardt_fxyz                   = C_3   | B_128  | FLT | XYZ,
    ardt_xyz                    = C_3   | B_192  | DBL | XYZ,
    ardt_xyz_polarisable        = C_3   | B_192  | DBL | XYZ  | POL,
    ardt_fxyza                  = C_4   | B_128  | FLT | XYZA,
    ardt_xyza                   = C_4   | B_256  | DBL | XYZA,
    ardt_xyza_polarisable       = C_4   | B_256  | DBL | XYZA | POL,

    ardt_fspectrum8             = C_8   | NOINFO | FLT | SPC,
    ardt_spectrum8              = C_8   | NOINFO | DBL | SPC,
    ardt_spectrum8_polarisable  = C_8   | NOINFO | DBL | SPC | POL,
    ardt_fspectrum11            = C_11  | NOINFO | FLT | SPC,
    ardt_spectrum11             = C_11  | NOINFO | DBL | SPC,
    ardt_spectrum11_polarisable = C_11  | NOINFO | DBL | SPC | POL,
    ardt_fspectrum18            = C_18  | NOINFO | FLT | SPC,
    ardt_spectrum18             = C_18  | NOINFO | DBL | SPC,
    ardt_spectrum18_polarisable = C_18  | NOINFO | DBL | SPC | POL,
    ardt_fspectrum46            = C_46  | NOINFO | FLT | SPC,
    ardt_spectrum46             = C_46  | NOINFO | DBL | SPC,
    ardt_spectrum46_polarisable = C_46  | NOINFO | DBL | SPC | POL,
    ardt_spectrum500            = C_500 | NOINFO | DBL | SPC,
    ardt_spectrum500_polarisable= C_500 | NOINFO | DBL | SPC | POL,

    ardt_all_bits_set           = 0xffffffff,
}
ArDataType;

//   And this file contains #undef to get rid of the shortcuts used above.
//   The various shortcuts used there have far too generic names to be
//   allowed to exist outside this header.

#include "ArDataType_PostDefinitionMacros.h"

const char * ardatatype_name(
        ArDataType  dataType
        );

const char * ardatatype_long_name_string(
        ART_GV      * art_gv,
        ArDataType    dataType
        );

const char * ardatatype_polarisable_string(
        ArDataType  dataType
        );


#define ARDATATYPE_NUMCHANNELS(__act) \
    (((__act) & ardt_num_channels_mask ) >> 12 )

unsigned int ardatatype_channels(
        ArDataType  dataType
        );

//   You should really test what this function returns to you. If it
//   can't make sense of the string you pass it, it will quietly hand you
//   a return value of 'ardt_unknown'. Which might not what your code
//   expects to work with.

ArDataType ardatatype_from_string(
        const char  * datatype_string
        );


#endif /* _ART_FOUNDATION_COLOURANDSPECTRA_ARDATATYPE_H_ */
/* ======================================================================== */
