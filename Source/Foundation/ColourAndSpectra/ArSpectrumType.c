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

#include "ArSpectrumType.h"

#include "ArUntaggedRGB.h"
#include "ArSpectrum8.h"
#include "ArSpectrum11.h"
#include "ArSpectrum18.h"
#include "ArSpectrum46.h"

typedef struct ArSpectrumTypeMap
{
    unsigned int        type;
    const char *        name;
}
ArSpectrumTypeMap;

const ArSpectrumTypeMap spectrumTypeMap[] =
{
    { arspectrum_grey1                 , "arspectrum_grey1" },
    { arspectrum_grey2                 , "arspectrum_grey2" },
    { arspectrum_grey4                 , "arspectrum_grey4" },
    { arspectrum_grey8                 , "arspectrum_grey8" },
    { arspectrum_grey16                , "arspectrum_grey16" },
    { arspectrum_grey32                , "arspectrum_grey32" },
    { arspectrum_grey64                , "arspectrum_grey64" },
    { arspectrum_grey1_negative        , "arspectrum_grey1_negative" },
    { arspectrum_grey2_negative        , "arspectrum_grey2_negative" },
    { arspectrum_grey4_negative        , "arspectrum_grey4_negative" },
    { arspectrum_grey8_negative        , "arspectrum_grey8_negative" },
    { arspectrum_grey16_negative       , "arspectrum_grey16_negative" },
    { arspectrum_grey32_negative       , "arspectrum_grey32_negative" },
    { arspectrum_grey64_negative       , "arspectrum_grey64_negative" },
    { arspectrum_rgb12                 , "arspectrum_rgb12" },
    { arspectrum_rgb24                 , "arspectrum_rgb24" },
    { arspectrum_rgb24falsecolour      , "arspectrum_rgb24falsecolour" },
    { arspectrum_rgb48                 , "arspectrum_rgb48" },
    { arspectrum_rgb48falsecolour      , "arspectrum_rgb48falsecolour" },
    { arspectrum_rgb96                 , "arspectrum_rgb96" },
    { arspectrum_rgb192                , "arspectrum_rgb192" },
    { arspectrum_rgb16                 , "arspectrum_rgb16" },
    { arspectrum_rgba32                 , "arspectrum_rgba32" },
    { arspectrum_rgba64                 , "arspectrum_rgba64" },
    { arspectrum_rgba128                , "arspectrum_rgba128" },
    { arspectrum_rgb256                , "arspectrum_rgb256" },
    { arspectrum_fgrey                 , "arspectrum_fgrey" },
    { arspectrum_grey                  , "arspectrum_grey" },
    { arspectrum_frgb                  , "arspectrum_frgb" },
    { arspectrum_rgb                   , "arspectrum_rgb" },
    { arspectrum_ut_rgb                , "arspectrum_ut_rgb" },
    { arspectrum_ut_rgb_polarisable    , "arspectrum_ut_rgb_polarisable" },
    { arspectrum_frgba                 , "arspectrum_frgba" },
    { arspectrum_rgba                  , "arspectrum_rgba" },
    { arspectrum_fciexyz               , "arspectrum_fciexyz" },
    { arspectrum_ciexyz                , "arspectrum_ciexyz" },
    { arspectrum_ciexyz_polarisable    , "arspectrum_ciexyz_polarisable" },
    { arspectrum_fspectrum8            , "arspectrum_fspectrum8" },
    { arspectrum_spectrum8             , "arspectrum_spectrum8" },
    { arspectrum_spectrum8_polarisable , "arspectrum_spectrum8_polarisable" },
    { arspectrum_fspectrum11           , "arspectrum_fspectrum11" },
    { arspectrum_spectrum11            , "arspectrum_spectrum11" },
    { arspectrum_spectrum11_polarisable, "arspectrum_spectrum11_polarisable" },
    { arspectrum_fspectrum18           , "arspectrum_fspectrum18" },
    { arspectrum_spectrum18            , "arspectrum_spectrum18" },
    { arspectrum_spectrum18_polarisable, "arspectrum_spectrum18_polarisable" },
    { arspectrum_fspectrum46           , "arspectrum_fspectrum46" },
    { arspectrum_spectrum46            , "arspectrum_spectrum46" },
    { arspectrum_spectrum46_polarisable, "arspectrum_spectrum46_polarisable" },
    { arspectrum_logluv                , "arspectrum_logluv" },

    { arspectrum_unknown               , "arspectrum_unknown" }  /* must be last */
};

const char * arspectrumtype_name(
        ArSpectrumType  spectrumType
        )
{
    unsigned int  i = 0;

    while ( spectrumTypeMap[i].type != spectrumType )
    {
        if ( spectrumTypeMap[i].type == arspectrum_unknown)  break;
        i++;
    }

    return spectrumTypeMap[i].name;
}

const char * arspectrumtype_long_name_string(
        ART_GV          * art_gv,
        ArSpectrumType    spectrumType
        )
{
    switch ( spectrumType )
    {
        case arspectrum_ut_rgb:
        case arspectrum_ut_rgb_polarisable:
            return ut_rgb_typename_string(art_gv);
            break;

        case arspectrum_spectrum8:
        case arspectrum_spectrum8_polarisable:
            return s8_typename_string(art_gv);
            break;

        case arspectrum_spectrum11:
        case arspectrum_spectrum11_polarisable:
            return s11_typename_string(art_gv);
            break;

        case arspectrum_spectrum18:
        case arspectrum_spectrum18_polarisable:
            return s18_typename_string(art_gv);
            break;

        case arspectrum_spectrum46:
        case arspectrum_spectrum46_polarisable:
            return s46_typename_string(art_gv);
            break;

        default:
            ART_ERRORHANDLING_FATAL_ERROR(
                "invalid ArSpectrumType"
                );
            break;
    }

    return "";
}

const char * arspectrumtype_polarisable_string(
        ArSpectrumType  spectrumType
        )
{
    if ( spectrumType & arspectrum_polarisable )
        return "polarisable ";
    else
        return "plain ";
}

unsigned int arspectrumtype_channels(
        ArSpectrumType  spectrumType
        )
{
    return ( spectrumType & arspectrum_channels_mask ) >> 8;
}

unsigned int arspectrumtype_bits(
        ArSpectrumType  spectrumType
        )
{
    return ( spectrumType & arspectrum_bits_mask );
}

/* ======================================================================== */
