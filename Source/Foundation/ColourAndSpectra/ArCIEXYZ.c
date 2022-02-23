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

#define ART_MODULE_NAME     ArCIEXYZ

#include "ArCIEXYZ.h"

#include "ArCIEColourConversions.h"
#include "ColourAndSpectralDataConversion.h"
#include "SpectralDatatype_ImplementationMacros.h"

#include "ArSpectrum500.h"

CANONICAL_GV_FOR_ISR_WITH_ADDITIONAL_FIELDS(
    ArCIEXYZ,
    ArCIEXYZ           XYZ_E;
    ArCIEXYZ           XYZ_A;
    ArCIEXYZ           XYZ_D50;
    ArCIEXYZ           XYZ_D55;
    ArCIEXYZ           XYZ_D60;
    ArCIEXYZ           XYZ_D65;
    ArCIEXYZ           XYZ_D75;
)

#define _ISR_CHANNELS               3

#define XYZ_GV                      art_gv->arciexyz_gv
#define XYZ_CHANNELS                XYZ_GV->channels
#define XYZ_FIRST_VISIBLE_CHANNEL   XYZ_GV->first_visible_channel
#define XYZ_SAMPLE_BOUND            XYZ_GV->sample_bound
#define XYZ_SAMPLING_RANGE          XYZ_GV->sampling_range
#define XYZ_SAMPLE_CENTER           XYZ_GV->sample_center
#define XYZ_SAMPLE_WIDTH            XYZ_GV->sample_width
#define XYZ_SAMPLE_WIDTH_DIV_2      XYZ_GV->sample_width_div_2
#define XYZ_SAMPLE_WEIGHT           XYZ_GV->sample_weight
#define XYZ_SAMPLE_BASIS            XYZ_GV->sample_basis
#define XYZ_SHORTNAME_STRING        XYZ_GV->shortname_string
#define XYZ_TYPENAME_STRING         XYZ_GV->typename_string
#define XYZ_DESCRIPTION_STRING      XYZ_GV->description_string
#define XYZ_ZERO                    XYZ_GV->zero
#define XYZ_UNIT                    XYZ_GV->unit
#define XYZ_COMPUTATION_SPACE       XYZ_GV->computation_space
#define XYZ_E                       XYZ_GV->XYZ_E
#define XYZ_A                       XYZ_GV->XYZ_A
#define XYZ_D50                     XYZ_GV->XYZ_D50
#define XYZ_D55                     XYZ_GV->XYZ_D55
#define XYZ_D60                     XYZ_GV->XYZ_D60
#define XYZ_D65                     XYZ_GV->XYZ_D65
#define XYZ_D75                     XYZ_GV->XYZ_D75

ART_MODULE_INITIALISATION_FUNCTION
(
    XYZ_GV = ALLOC( ArCIEXYZ_GV );

    XYZ_CHANNELS = _ISR_CHANNELS;
    XYZ_FIRST_VISIBLE_CHANNEL = 0;

    XYZ_SAMPLE_BOUND = ALLOC_ARRAY( double, XYZ_CHANNELS + 1 );

    double  range_start = 380.0 NM;
    double  channel_width = 120.0 NM;

    for ( unsigned int i = 0; i < (XYZ_CHANNELS + 1); i++ )
    {
        XYZ_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    XYZ_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            XYZ_FIRST_VISIBLE_CHANNEL = i;
    }
 
    XYZ_FIRST_VISIBLE_CHANNEL = 0;
    XYZ_SAMPLE_BOUND[0] = 380 NM;
    XYZ_SAMPLE_BOUND[1] = 520 NM;
    XYZ_SAMPLE_BOUND[2] = 680 NM;
    XYZ_SAMPLE_BOUND[3] = 780 NM;

    XYZ_SAMPLING_RANGE = 400 NM;

    double  uniform_channel_width = XYZ_SAMPLING_RANGE / XYZ_CHANNELS;

    XYZ_SAMPLE_CENTER      = ALLOC_ARRAY( double, XYZ_CHANNELS );
    XYZ_SAMPLE_WIDTH       = ALLOC_ARRAY( double, XYZ_CHANNELS );
    XYZ_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, XYZ_CHANNELS );
    XYZ_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, XYZ_CHANNELS );
    XYZ_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, XYZ_CHANNELS );

    for ( unsigned int i = 0; i < XYZ_CHANNELS; i++ )
    {
        XYZ_SAMPLE_CENTER[i] =
            ( XYZ_SAMPLE_BOUND[ i + 1 ] + XYZ_SAMPLE_BOUND[ i ] ) / 2.0;

        XYZ_SAMPLE_WIDTH[i] =
            XYZ_SAMPLE_BOUND[ i + 1 ] - XYZ_SAMPLE_BOUND[ i ];

        XYZ_SAMPLE_WIDTH_DIV_2[i] = XYZ_SAMPLE_WIDTH[i] / 2.0;

        if ( XYZ_SAMPLE_WIDTH[i] == 0. )
        {
            XYZ_SAMPLE_WEIGHT[i] = 0.;
            ART_ERRORHANDLING_WARNING(
                "zero sample width - this is almost certainly not intended, "
                "and should be fixed!");
        }
        else
        {
            XYZ_SAMPLE_WEIGHT[i] = uniform_channel_width / XYZ_SAMPLE_WIDTH[i];
        }

        XYZ_SAMPLE_BASIS[i].size  = 2;
        XYZ_SAMPLE_BASIS[i].scale =
            1.0 / (  XYZ_SAMPLE_BOUND[ i + 1 ]
                   - XYZ_SAMPLE_BOUND[ i     ] );

        XYZ_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        XYZ_SAMPLE_BASIS[i].array[0] = PNT2D( XYZ_SAMPLE_BOUND[ i    ], 1.0 );
        XYZ_SAMPLE_BASIS[i].array[1] = PNT2D( XYZ_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    XYZ_SHORTNAME_STRING = "xyz";
    XYZ_TYPENAME_STRING = "CIE XYZ";
    XYZ_DESCRIPTION_STRING =
        "visible range, colourspace";

    XYZ_ZERO = xyz_d_alloc_init( art_gv, 0.0 );
    XYZ_UNIT = xyz_d_alloc_init( art_gv, 1.0 );

    ArCIExyY  xyY_E   = ARCIExyY_xy1( ARCIEXY_E );
    ArCIExyY  xyY_A   = ARCIExyY_xy1( ARCIEXY_A );
    ArCIExyY  xyY_D50 = ARCIExyY_xy1( ARCIEXY_D50 );
    ArCIExyY  xyY_D55 = ARCIExyY_xy1( ARCIEXY_D55 );
    ArCIExyY  xyY_D60 = ARCIExyY_xy1( ARCIEXY_D60 );
    ArCIExyY  xyY_D65 = ARCIExyY_xy1( ARCIEXY_D65 );
    ArCIExyY  xyY_D75 = ARCIExyY_xy1( ARCIEXY_D75 );

    xyy_to_xyz( art_gv, & xyY_E  , & XYZ_E   );
    xyy_to_xyz( art_gv, & xyY_A  , & XYZ_A );
    xyy_to_xyz( art_gv, & xyY_D50, & XYZ_D50 );
    xyy_to_xyz( art_gv, & xyY_D55, & XYZ_D55 );
    xyy_to_xyz( art_gv, & xyY_D60, & XYZ_D60 );
    xyy_to_xyz( art_gv, & xyY_D65, & XYZ_D65 );
    xyy_to_xyz( art_gv, & xyY_D75, & XYZ_D75 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(XYZ_SAMPLE_BOUND);
    FREE_ARRAY(XYZ_SAMPLE_CENTER);
    FREE_ARRAY(XYZ_SAMPLE_WIDTH);
    FREE_ARRAY(XYZ_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(XYZ_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < XYZ_CHANNELS; i++ )
        pss_freearray_s( art_gv, & XYZ_SAMPLE_BASIS[i] );

    FREE_ARRAY(XYZ_SAMPLE_BASIS);

    xyz_free( art_gv, XYZ_ZERO );
    xyz_free( art_gv, XYZ_UNIT );

    FREE(XYZ_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C3_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArCIEXYZ, arciexyz, ARCIEXYZ, xyz, c3, s );

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI


void xyz_ss_convolve_d(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c0,
        const ArCIEXYZ  * c1,
              double    * dr
        )
{
    (void) art_gv;
    (void) c0;
    (void) c1;
    (void) dr;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "convolution operation not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );
}

//   Be aware of what this does: here, NO spectral uplifting is performed
//   Instead, we go to RGB, and pick the channel which is most appropriate
//   for the selected wavelength.

//   The two boundaries defined here are a rough estimate: but if one is
//   picking spectral channels from an XYZ image, any excess accuracy is
//   unwarranted anyway.

#define  BOUNDARY_BG    490 NM
#define  BOUNDARY_GR    590 NM

double xyz_sd_value_at_wavelength(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0,
        const double      d_0
        )
{
    ArRGB  rgb;
    
    xyz_conversion_to_linear_rgb(
          art_gv,
          c_0,
        & rgb
        );
    
    double  result = ARRGB_B(*c_0);
    
    if ( d_0 > BOUNDARY_BG )
    {
        if ( d_0 > BOUNDARY_GR )
        {
            result = ARRGB_R(*c_0);
        }
        else
        {
            result = ARRGB_G(*c_0);
        }
    }

    return result;
}

void xyz_sdd_sample_at_wavelength_s(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0,
        const double      d_0,
        const double      d_1,
              ArCIEXYZ  * c_r
        )
{
    (void) d_1;
    //   Note: this is probably not what should be done: a gradual shift
    //         between channels would likely be much better.
    
    ArRGB  rgb;
    
    xyz_conversion_to_linear_rgb(
          art_gv,
          c_0,
        & rgb
        );
    
    ArRGB  result = ARRGB(0,0,0);
    
    if ( d_0 > BOUNDARY_BG )
    {
        BC(result) = ARRGB_B(*c_0);
    }
    
    if ( d_0 > BOUNDARY_BG )
    {
        if ( d_0 > BOUNDARY_GR )
        {
            RC(result) = ARRGB_R(*c_0);
        }
        else
        {
            GC(result) = ARRGB_G(*c_0);
        }
    }
    
    rgb_to_xyz(
          art_gv,
        & result,
          c_r
        );
}

double xyz_ss_convolve(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c0,
        const ArCIEXYZ  * c1
        )
{
    (void) art_gv;
    (void) c0;
    (void) c1;
    
    ART_ERRORHANDLING_FATAL_ERROR(
        "ArSpectrum value convolution not defined in colour space - "
        "switch ART to a spectral ISR to avoid this error"
        );

    return 0.0;
}

void xyz_s_debugprintf(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0
        )
{
    (void) art_gv;
    
    printf( "ArCIEXYZ( % 5.3f, % 5.3f, % 5.3f )\n",
        ARRGB_R(*c_0),
        ARRGB_G(*c_0),
        ARRGB_B(*c_0)
        );

    fflush(stdout);
}

void xyz_s_mathematicaprintf(
        const ART_GV    * art_gv,
        const ArCIEXYZ  * c_0
        )
{
    (void) art_gv;
    
    printf( "ArCIEXYZ{ % 5.3f, % 5.3f, % 5.3f }\n",
        ARRGB_R(*c_0),
        ARRGB_G(*c_0),
        ARRGB_B(*c_0)
        );

    fflush(stdout);
}

ArCIEXYZ const * xyz_illuminant_E(
        const ART_GV  * art_gv
        )
{
    return & XYZ_E;
}

ArCIEXYZ const * xyz_illuminant_A(
        const ART_GV  * art_gv
        )
{
    return & XYZ_A;
}

ArCIEXYZ const * xyz_illuminant_D50(
        const ART_GV  * art_gv
        )
{
    return & XYZ_D50;
}

ArCIEXYZ const * xyz_illuminant_D55(
        const ART_GV  * art_gv
        )
{
    return & XYZ_D55;
}

ArCIEXYZ const * xyz_illuminant_D60(
        const ART_GV  * art_gv
        )
{
    return & XYZ_D60;
}

ArCIEXYZ const * xyz_illuminant_D65(
        const ART_GV  * art_gv
        )
{
    return & XYZ_D65;
}

ArCIEXYZ const * xyz_illuminant_D75(
        const ART_GV  * art_gv
        )
{
    return & XYZ_D75;
}

/* ======================================================================== */
