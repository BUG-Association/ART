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

#define ART_MODULE_NAME     ArSpectrum500

#include "ART_Foundation_Geometry.h"
#include "ArSpectrum500.h"

#include "SpectralDatatype_ImplementationMacros.h"

CANONICAL_GV_FOR_ISR(ArSpectrum500)

#define _ISR_CHANNELS            500

#define S500_GV                     art_gv->arspectrum500_gv
#define S500_CHANNELS               S500_GV->channels
#define S500_FIRST_VISIBLE_CHANNEL  S500_GV->first_visible_channel
#define S500_SAMPLE_BOUND           S500_GV->sample_bound
#define S500_SAMPLING_RANGE         S500_GV->sampling_range
#define S500_SAMPLE_CENTER          S500_GV->sample_center
#define S500_SAMPLE_WIDTH           S500_GV->sample_width
#define S500_SAMPLE_WIDTH_DIV_2     S500_GV->sample_width_div_2
#define S500_SAMPLE_WEIGHT          S500_GV->sample_weight
#define S500_SAMPLE_BASIS           S500_GV->sample_basis
#define S500_SHORTNAME_STRING       S500_GV->shortname_string
#define S500_TYPENAME_STRING        S500_GV->typename_string
#define S500_DESCRIPTION_STRING     S500_GV->description_string
#define S500_ZERO                   S500_GV->zero
#define S500_UNIT                   S500_GV->unit
#define S500_COMPUTATION_SPACE      S500_GV->computation_space

ART_MODULE_INITIALISATION_FUNCTION
(
    S500_GV = ALLOC( ArSpectrum500_GV );

    S500_CHANNELS = _ISR_CHANNELS;
    S500_FIRST_VISIBLE_CHANNEL = -1;   //   placeholder - determined later

    S500_SAMPLE_BOUND = ALLOC_ARRAY( double, S500_CHANNELS + 1 );

    double  range_start = 300.0 NM;
    double  channel_width = 1.0 NM;

    for ( unsigned int i = 0; i < (S500_CHANNELS + 1); i++ )
    {
        S500_SAMPLE_BOUND[i] = range_start + i * channel_width;

        double  next_sample_bound =
            range_start + (i+1) * channel_width;

        //   we already count a channel towards the visible range if it
        //   partially lies within it

        if (    S500_FIRST_VISIBLE_CHANNEL == -1
             && next_sample_bound > 380.0 NM )
            S500_FIRST_VISIBLE_CHANNEL = i;
    }

    S500_SAMPLING_RANGE =
        S500_SAMPLE_BOUND[S500_CHANNELS] - S500_SAMPLE_BOUND[0];

    S500_SAMPLE_CENTER      = ALLOC_ARRAY( double, S500_CHANNELS );
    S500_SAMPLE_WIDTH       = ALLOC_ARRAY( double, S500_CHANNELS );
    S500_SAMPLE_WIDTH_DIV_2 = ALLOC_ARRAY( double, S500_CHANNELS );
    S500_SAMPLE_WEIGHT      = ALLOC_ARRAY( double, S500_CHANNELS );
    S500_SAMPLE_BASIS       = ALLOC_ARRAY( ArPSSpectrum, S500_CHANNELS );

    for ( unsigned int i = 0; i < S500_CHANNELS; i++ )
    {
        S500_SAMPLE_CENTER[i] =
            ( S500_SAMPLE_BOUND[ i + 1 ] + S500_SAMPLE_BOUND[ i ] ) / 2.0;

        S500_SAMPLE_WIDTH[i] =
            S500_SAMPLE_BOUND[ i + 1 ] - S500_SAMPLE_BOUND[ i ];

        S500_SAMPLE_WIDTH_DIV_2[i] = S500_SAMPLE_WIDTH[i] / 2.0;

        S500_SAMPLE_WEIGHT[i] = S500_SAMPLE_WIDTH[i] / S500_SAMPLING_RANGE;

        S500_SAMPLE_BASIS[i].size  = 2;
        S500_SAMPLE_BASIS[i].scale =
            1.0 / (  S500_SAMPLE_BOUND[ i + 1 ]
                   - S500_SAMPLE_BOUND[ i     ] );

        S500_SAMPLE_BASIS[i].array = ALLOC_ARRAY( Pnt2D, 2 );
        S500_SAMPLE_BASIS[i].array[0] = PNT2D( S500_SAMPLE_BOUND[ i    ], 1.0 );
        S500_SAMPLE_BASIS[i].array[1] = PNT2D( S500_SAMPLE_BOUND[ i + 1], 1.0 );
    }

    S500_SHORTNAME_STRING = "S500";
    S500_TYPENAME_STRING = "Spectrum 500";
    S500_DESCRIPTION_STRING =
        "near UV + visible + near IR range, 500 spectral samples, 1.0nm spacing";

    S500_ZERO = s500_d_alloc_init( art_gv, 0.0 );
    S500_UNIT = s500_d_alloc_init( art_gv, 1.0 );
)

ART_MODULE_SHUTDOWN_FUNCTION
(
    FREE_ARRAY(S500_SAMPLE_BOUND);
    FREE_ARRAY(S500_SAMPLE_CENTER);
    FREE_ARRAY(S500_SAMPLE_WIDTH);
    FREE_ARRAY(S500_SAMPLE_WIDTH_DIV_2);
    FREE_ARRAY(S500_SAMPLE_WEIGHT);

    for ( unsigned int i = 0; i < S500_CHANNELS; i++ )
        pss_freearray_s( art_gv, & S500_SAMPLE_BASIS[i] );

    FREE_ARRAY(S500_SAMPLE_BASIS);

    s500_free( art_gv, S500_ZERO );
    s500_free( art_gv, S500_UNIT );

    FREE(S500_GV);
)

#define _ISR_C(_s0)             (_s0).c
#define _ISR_CI(_s0,_i)         C500_CI(_ISR_C(_s0),(_i))

CANONICAL_IMPLEMENTATION_FOR_ISR( ArSpectrum500, arspectrum500, S500, s500, c500, s );

ADDITIONAL_IMPLEMENTATION_FOR_SPECTRAL_ISR( ArSpectrum500, S500, s500, c500, s );
ISR_VALUE_AT_WAVELENGTH_IMPLEMENTATION( ArSpectrum500, arspectrum500, S500, s500, c500, s );

double s500_dc_value_at_wavelength(
        const ART_GV         * art_gv,
        const double           d0,
        const ArSpectrum500  * s0
        )
{
    (void) art_gv;
    
    //  The beauty of s500 is that we just take the nearest wavelength,
    //  and be done with it. No interpolation, nothing. For graphics
    //  purposes (!), this is enough.
    
    int  i = NANO_FROM_UNIT(d0) - 300;
    
    return s0->c.x[i];
}

#undef _ISR_CHANNELS
#undef _ISR_C
#undef _ISR_CI

/* ======================================================================== */
