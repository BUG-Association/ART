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

#include "ArDataType.h"

#include "ArCIEXYZ.h"
#include "ArRGB.h"
#include "ArSpectrum8.h"
#include "ArSpectrum11.h"
#include "ArSpectrum18.h"
#include "ArSpectrum46.h"
#include "ArSpectrum500.h"

typedef struct ArDataTypeMap
{
    unsigned int        type;
    const char *        name;
}
ArDataTypeMap;

const ArDataTypeMap spectrumTypeMap[] =
{
    { ardt_grey8                 , "ardt_grey8" },
    { ardt_grey8_negative        , "ardt_grey8_negative" },
    { ardt_grey16                , "ardt_grey16" },
    { ardt_grey16_negative       , "ardt_grey16_negative" },
    { ardt_grey16_alpha          , "ardt_grey16_alpha" },
    { ardt_grey32                , "ardt_grey32" },
    { ardt_grey32_negative       , "ardt_grey32_negative" },
    { ardt_grey32_alpha          , "ardt_grey32_alpha" },
    { ardt_grey64                , "ardt_grey64" },
    { ardt_grey64_negative       , "ardt_grey64_negative" },
    { ardt_grey64_alpha          , "ardt_grey64_alpha" },
    { ardt_fgrey                 , "ardt_fgrey" },
    { ardt_grey                  , "ardt_grey" },
    { ardt_grey_alpha            , "ardt_grey_aplha" },

    { ardt_rgb24                 , "ardt_rgb24" },
    { ardt_rgb24_falsecolour     , "ardt_rgb24_falsecolour" },
    { ardt_rgba32                , "ardt_rgba32" },
    { ardt_rgba32_falsecolour    , "ardt_rgba32_falsecolour" },
    { ardt_rgba32_plusminus      , "ardt_rgba32_plusminus" },
    { ardt_rgb48                 , "ardt_rgb48" },
    { ardt_rgb48_falsecolour     , "ardt_rgb48_falsecolour" },
    { ardt_rgba64                , "ardt_rgba64" },
    { ardt_rgba64_falsecolour    , "ardt_rgba64_falsecolour" },
    { ardt_rgba64_plusminus      , "ardt_rgba64_plusminus" },
    { ardt_rgb96                 , "ardt_rgb96" },
    
    { ardt_rgb                   , "ardt_rgb" },
    { ardt_rgb_polarisable       , "ardt_rgb_polarisable" },
    { ardt_frgb                  , "ardt_frgb" },
    { ardt_rgb                   , "ardt_rgb" },
    { ardt_rgba                  , "ardt_rgba" },
    { ardt_rgba_polarisable      , "ardt_rgba_polarisable" },
    { ardt_frgba                 , "ardt_frgba" },
    { ardt_rgba                  , "ardt_rgba" },

    { ardt_xyz                   , "ardt_xyz" },
    { ardt_xyz_polarisable       , "ardt_xyz_polarisable" },
    { ardt_fxyz                  , "ardt_fxyz" },
    { ardt_xyz                   , "ardt_xyz" },
    { ardt_xyza                  , "ardt_xyza" },
    { ardt_xyza_polarisable      , "ardt_xyza_polarisable" },
    { ardt_fxyza                 , "ardt_fxyza" },
    { ardt_xyza                  , "ardt_xyza" },

    { ardt_fspectrum8            , "ardt_fspectrum8" },
    { ardt_spectrum8             , "ardt_spectrum8" },
    { ardt_spectrum8_polarisable , "ardt_spectrum8_polarisable" },
    { ardt_fspectrum11           , "ardt_fspectrum11" },
    { ardt_spectrum11            , "ardt_spectrum11" },
    { ardt_spectrum11_polarisable, "ardt_spectrum11_polarisable" },
    { ardt_fspectrum18           , "ardt_fspectrum18" },
    { ardt_spectrum18            , "ardt_spectrum18" },
    { ardt_spectrum18_polarisable, "ardt_spectrum18_polarisable" },
    { ardt_fspectrum46           , "ardt_fspectrum46" },
    { ardt_spectrum46            , "ardt_spectrum46" },
    { ardt_spectrum46_polarisable, "ardt_spectrum46_polarisable" },
    { ardt_spectrum500           , "ardt_spectrum500" },
    { ardt_spectrum500_polarisable, "ardt_spectrum500_polarisable" },

    { ardt_unknown               , "ardt_unknown" }  /* must be last */
};

const char * ardatatype_name(
        ArDataType  dataType
        )
{
    unsigned int  i = 0;

    while ( spectrumTypeMap[i].type != dataType )
    {
        if ( spectrumTypeMap[i].type == ardt_unknown)  break;
        i++;
    }

    return spectrumTypeMap[i].name;
}

const char * ardatatype_long_name_string(
        ART_GV      * art_gv,
        ArDataType    dataType
        )
{
    switch ( dataType )
    {
        case ardt_xyz:
        case ardt_xyz_polarisable:
            return xyz_typename_string(art_gv);
            break;

        case ardt_spectrum8:
        case ardt_spectrum8_polarisable:
            return s8_typename_string(art_gv);
            break;

        case ardt_spectrum11:
        case ardt_spectrum11_polarisable:
            return s11_typename_string(art_gv);
            break;

        case ardt_spectrum18:
        case ardt_spectrum18_polarisable:
            return s18_typename_string(art_gv);
            break;

        case ardt_spectrum46:
        case ardt_spectrum46_polarisable:
            return s46_typename_string(art_gv);
            break;

        case ardt_spectrum500:
        case ardt_spectrum500_polarisable:
            return s500_typename_string(art_gv);
            break;


        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid ArDataType"
                );
            break;
    }

    return "";
}

const char * ardatatype_polarisable_string(
        ArDataType  dataType
        )
{
    if ( dataType & ardt_polarisable )
        return "polarisable ";
    else
        return "plain ";
}

ArDataType ardatatype_from_string(
        const char  * datatype_string
        )
{
    ArDataType  result = ardt_unknown;
    
    if ( strcmp( datatype_string, "csp" ) == 0 )
        result = ardt_xyz;

    if ( strcmp( datatype_string, "xyz" ) == 0 )
        result = ardt_xyz;

    if ( strcmp( datatype_string, "s8v" ) == 0 )
        result = ardt_spectrum8;

    if ( strcmp( datatype_string, "s11e" ) == 0 )
        result = ardt_spectrum11;

    if ( strcmp( datatype_string, "s18v" ) == 0 )
        result = ardt_spectrum18;

    if ( strcmp( datatype_string, "s46e" ) == 0 )
        result = ardt_spectrum46;

    return  result;
}

/* ======================================================================== */
